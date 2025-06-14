#include "design_graph.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <uhdm_file>" << std::endl;
        return 1;
    }

    std::string uhdmFile = argv[1];
    verilog_simulator::DesignGraph graph;

    // Build the graph from UHDM data
    if (!graph.buildFromUHDM(uhdmFile))
    {
        std::cerr << "Failed to build graph from UHDM file" << std::endl;
        return 1;
    }

    // Print the graph structure
    std::cout << "\nDesign Graph Structure:" << std::endl;
    std::cout << "======================" << std::endl;

    for (const auto &[name, node] : graph.getNodes())
    {
        std::cout << "\nNode: " << name << " (";
        switch (node->getType())
        {
        case verilog_simulator::Node::Type::GATE:
            std::cout << "GATE";
            break;
        case verilog_simulator::Node::Type::PORT:
            std::cout << "PORT";
            break;
        case verilog_simulator::Node::Type::NET:
            std::cout << "NET";
            break;
        case verilog_simulator::Node::Type::CONSTANT:
            std::cout << "CONSTANT";
            break;
        }
        std::cout << ")" << std::endl;

        std::cout << "  Inputs: ";
        for (const auto &input : node->getInputs())
        {
            std::cout << input->getSource()->getName() << " ";
        }
        std::cout << std::endl;

        std::cout << "  Outputs: ";
        for (const auto &output : node->getOutputs())
        {
            std::cout << output->getTarget()->getName() << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}