#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <uhdm/uhdm.h>
#include <uhdm/vpi_uhdm.h>

namespace verilog_simulator
{

    // Forward declarations
    class Node;
    class Edge;

    // Represents a node in the design graph (e.g., gates, ports, nets)
    class Node
    {
    public:
        enum class Type
        {
            GATE,    // Logic gates (AND, OR, etc.)
            PORT,    // Module ports
            NET,     // Wires/nets
            CONSTANT // Constant values
        };

        Node(const std::string &name, Type type);

        const std::string &getName() const { return name_; }
        Type getType() const { return type_; }
        const std::vector<std::shared_ptr<Edge>> &getInputs() const { return inputs_; }
        const std::vector<std::shared_ptr<Edge>> &getOutputs() const { return outputs_; }

        void addInput(std::shared_ptr<Edge> edge);
        void addOutput(std::shared_ptr<Edge> edge);

    private:
        std::string name_;
        Type type_;
        std::vector<std::shared_ptr<Edge>> inputs_;
        std::vector<std::shared_ptr<Edge>> outputs_;
    };

    // Represents a connection between nodes
    class Edge
    {
    public:
        Edge(std::shared_ptr<Node> source, std::shared_ptr<Node> target);

        std::shared_ptr<Node> getSource() const { return source_; }
        std::shared_ptr<Node> getTarget() const { return target_; }

    private:
        std::shared_ptr<Node> source_;
        std::shared_ptr<Node> target_;
    };

    // The main design graph class
    class DesignGraph
    {
    public:
        DesignGraph() = default;

        // Add a node to the graph
        std::shared_ptr<Node> addNode(const std::string &name, Node::Type type);

        // Add an edge between two nodes
        std::shared_ptr<Edge> addEdge(const std::string &sourceName, const std::string &targetName);

        // Get a node by name
        std::shared_ptr<Node> getNode(const std::string &name) const;

        // Get all nodes
        const std::unordered_map<std::string, std::shared_ptr<Node>> &getNodes() const { return nodes_; }

        // Build the graph from UHDM data
        bool buildFromUHDM(const std::string &uhdmFile);

    private:
        std::unordered_map<std::string, std::shared_ptr<Node>> nodes_;
        std::vector<std::shared_ptr<Edge>> edges_;

        // Helper methods for processing UHDM data
        void processModule(UHDM::module_inst *module);
        void processPort(UHDM::port *port);
        void processNet(UHDM::net *net);
        void processGate(UHDM::module_inst *gate);
        void processContAssign(UHDM::cont_assign *assign);

        // Recursively collect net/signal names referenced by an expression
        void collectSignalNames(UHDM::any *expr, std::vector<std::string> &names);
    };
}