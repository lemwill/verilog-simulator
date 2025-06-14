#include "design_graph.hpp"
#include <iostream>

namespace verilog_simulator
{

    Node::Node(const std::string &name, Type type)
        : name_(name), type_(type) {}

    void Node::addInput(std::shared_ptr<Edge> edge)
    {
        inputs_.push_back(edge);
    }

    void Node::addOutput(std::shared_ptr<Edge> edge)
    {
        outputs_.push_back(edge);
    }

    Edge::Edge(std::shared_ptr<Node> source, std::shared_ptr<Node> target)
        : source_(std::move(source)), target_(std::move(target)) {}

    std::shared_ptr<Node> DesignGraph::addNode(const std::string &name, Node::Type type)
    {
        auto node = std::make_shared<Node>(name, type);
        nodes_[name] = node;
        return node;
    }

    std::shared_ptr<Edge> DesignGraph::addEdge(const std::string &sourceName, const std::string &targetName)
    {
        auto source = getNode(sourceName);
        auto target = getNode(targetName);

        if (!source || !target)
        {
            std::cerr << "Error: Cannot create edge between " << sourceName << " and " << targetName
                      << " - one or both nodes not found" << std::endl;
            return nullptr;
        }

        auto edge = std::make_shared<Edge>(source, target);

        // Connect to nodes
        source->addOutput(edge);
        target->addInput(edge);

        edges_.push_back(edge);
        return edge;
    }

    std::shared_ptr<Node> DesignGraph::getNode(const std::string &name) const
    {
        auto it = nodes_.find(name);
        return it != nodes_.end() ? it->second : nullptr;
    }

    bool DesignGraph::buildFromUHDM(const std::string &uhdmFile)
    {
        // Create a serializer
        UHDM::Serializer serializer;

        // Restore the UHDM database
        auto handles = serializer.Restore(uhdmFile);
        if (handles.empty())
        {
            std::cerr << "Failed to load UHDM database: " << uhdmFile << std::endl;
            return false;
        }

        // Get the design from the first handle
        auto design = UhdmDesignFromVpiHandle(handles[0]);
        if (!design)
        {
            std::cerr << "Failed to extract design from UHDM database" << std::endl;
            return false;
        }

        // Process all top-level modules
        for (auto module : *design->TopModules())
        {
            processModule(module);
        }

        return true;
    }

    void DesignGraph::processModule(UHDM::module_inst *module)
    {
        std::cout << "Processing module: " << std::string(module->VpiName()) << std::endl;

        // Process ports
        for (auto port : *module->Ports())
        {
            processPort(port);
        }

        // Process nets
        for (auto net : *module->Nets())
        {
            processNet(net);
        }

        // Process module instances (sub-modules / gates)
        if (auto subInst = module->Module_inst())
        {
            processGate(subInst);
        }

        // Process continuous assignments
        for (auto assign : *module->Cont_assigns())
        {
            processContAssign(assign);
        }
    }

    void DesignGraph::processPort(UHDM::port *port)
    {
        std::string portName(port->VpiName());
        addNode(portName, Node::Type::PORT);
        std::cout << "Found port: " << portName << std::endl;
    }

    void DesignGraph::processNet(UHDM::net *net)
    {
        std::string netName(net->VpiName());
        addNode(netName, Node::Type::NET);
        std::cout << "Found net: " << netName << std::endl;
    }

    void DesignGraph::processGate(UHDM::module_inst *gate)
    {
        std::string gateName(gate->VpiName());
        addNode(gateName, Node::Type::GATE);
        std::cout << "Found gate: " << gateName << std::endl;

        // Process gate connections through ports
        for (auto port : *gate->Ports())
        {
            std::string portName(port->VpiName());
            if (port->High_conn())
            {
                std::string netName(port->High_conn()->VpiName());

                if (portName == "o" || portName == "out")
                {
                    addEdge(gateName, netName);
                }
                else
                {
                    addEdge(netName, gateName);
                }
            }
        }
    }

    void DesignGraph::collectSignalNames(UHDM::any *expr, std::vector<std::string> &names)
    {
        if (!expr)
            return;

        const char *debugEnv = std::getenv("VERILOG_SIM_DEBUG");
        bool debug = debugEnv && std::string(debugEnv) == "1";
        if (debug)
        {
            std::cout << "collectSignalNames: UhdmType=" << expr->UhdmType()
                      << " VpiType=" << expr->VpiType()
                      << " name='" << expr->VpiName() << "'" << std::endl;
        }

        switch (expr->VpiType())
        {
        case vpiNet:
        case vpiPort:
        case vpiReg:
        case vpiWire:
        case vpiRefVar:
        case vpiRefObj:
        {
            std::string n(expr->VpiName());
            if (!n.empty())
                names.push_back(n);
            break;
        }
        default:
            break;
        }

        // Recurse into operands (for operations) if available
        if (auto op = dynamic_cast<UHDM::operation *>(expr))
        {
            if (auto ops = op->Operands())
            {
                for (auto operand : *ops)
                {
                    collectSignalNames(operand, names);
                }
            }
        }

        // For other expression types (bit_select, part_select, etc.) we rely on VpiName or operand traversal above.

        if (debug)
        {
            std::cout << "collectSignalNames: collected " << names.size() << " names" << std::endl;
        }
    }

    void DesignGraph::processContAssign(UHDM::cont_assign *assign)
    {
        std::string lhsName(assign->Lhs()->VpiName());
        addNode(lhsName, Node::Type::NET);

        // Collect source signal names from RHS expression
        std::vector<std::string> sources;
        collectSignalNames(assign->Rhs(), sources);

        if (sources.empty())
        {
            std::cerr << "Warning: RHS of assignment to " << lhsName << " has no identifiable signals" << std::endl;
        }

        for (const auto &src : sources)
        {
            addEdge(src, lhsName);
            std::cout << "Found continuous assignment: " << src << " -> " << lhsName << std::endl;
        }
    }

} // namespace verilog_simulator