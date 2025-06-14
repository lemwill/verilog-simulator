#include "node.hpp"
#include "edge.hpp"

namespace verilog_simulator
{

    Node::Node(const std::string &name, Type type) : name_(name), type_(type) {}

    void Node::addInput(const std::shared_ptr<Edge> &e) { inputs_.push_back(e); }
    void Node::addOutput(const std::shared_ptr<Edge> &e) { outputs_.push_back(e); }

} // namespace verilog_simulator