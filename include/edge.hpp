#pragma once

#include <memory>

namespace verilog_simulator
{
    class Node;

    class Edge
    {
    public:
        Edge(std::shared_ptr<Node> src, std::shared_ptr<Node> dst);

        std::shared_ptr<Node> getSource() const { return source_; }
        std::shared_ptr<Node> getTarget() const { return target_; }

    private:
        std::shared_ptr<Node> source_;
        std::shared_ptr<Node> target_;
    };

} // namespace verilog_simulator