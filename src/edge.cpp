#include "edge.hpp"
#include "node.hpp"

namespace verilog_simulator
{

    Edge::Edge(std::shared_ptr<Node> src, std::shared_ptr<Node> dst) : source_(std::move(src)), target_(std::move(dst)) {}

} // namespace verilog_simulator