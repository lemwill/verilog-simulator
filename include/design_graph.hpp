#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <uhdm/uhdm.h>
#include <uhdm/vpi_uhdm.h>

#include "node.hpp"
#include "edge.hpp"
#include "logic.hpp"

namespace verilog_simulator
{

    // Forward declarations removed; now using actual classes

    class DesignGraph
    {
    public:
        DesignGraph() = default;

        std::shared_ptr<Node> addNode(const std::string &name, Node::Type type);
        std::shared_ptr<Edge> addEdge(const std::string &sourceName, const std::string &targetName);
        std::shared_ptr<Node> getNode(const std::string &name) const;
        const std::unordered_map<std::string, std::shared_ptr<Node>> &getNodes() const { return nodes_; }

        bool buildFromUHDM(const std::string &uhdmFile);

        void setInputs(const std::unordered_map<std::string, Logic> &inputs);
        void evaluate();
        Logic getValue(const std::string &nodeName) const;

        std::string toDot() const;
        std::string toJson() const;
        bool saveDot(const std::string &filename) const;
        bool saveJson(const std::string &filename) const;

    private:
        struct Assignment
        {
            std::string lhs;
            std::vector<std::string> rhs;
        };

        std::unordered_map<std::string, std::shared_ptr<Node>> nodes_;
        std::vector<std::shared_ptr<Edge>> edges_;
        std::vector<Assignment> assignments_;

        void processModule(UHDM::module_inst *module);
        void processPort(UHDM::port *port);
        void processNet(UHDM::net *net);
        void processGate(UHDM::module_inst *gate);
        void processContAssign(UHDM::cont_assign *assign);
        void collectSignalNames(UHDM::any *expr, std::vector<std::string> &names);
        void evaluateCombinational();
    };
}