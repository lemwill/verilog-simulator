#pragma once

#include "logic.hpp"
#include <string>
#include <vector>
#include <memory>

namespace verilog_simulator
{

    class Edge; // fwd

    class Node
    {
    public:
        enum class Type
        {
            GATE,
            PORT,
            NET,
            CONSTANT
        };

        Node(const std::string &name, Type type);

        const std::string &getName() const { return name_; }
        Type getType() const { return type_; }

        void addInput(const std::shared_ptr<Edge> &e);
        void addOutput(const std::shared_ptr<Edge> &e);

        const std::vector<std::shared_ptr<Edge>> &getInputs() const { return inputs_; }
        const std::vector<std::shared_ptr<Edge>> &getOutputs() const { return outputs_; }

        // Logic helpers
        void setValue(Logic v) { value_ = v; }
        Logic getValue() const { return value_; }

    private:
        std::string name_;
        Type type_;
        std::vector<std::shared_ptr<Edge>> inputs_;
        std::vector<std::shared_ptr<Edge>> outputs_;
        Logic value_{Logic::X};
    };

} // namespace verilog_simulator