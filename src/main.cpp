#include "design_graph.hpp"
#include "logic.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <uhdm_file> [signal=value ...]" << std::endl;
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

    // Parse optional input signal assignments (signal=0/1)
    std::unordered_map<std::string, verilog_simulator::Logic> inputs;
    for (int i = 2; i < argc; ++i)
    {
        std::string token = argv[i];
        auto eqPos = token.find('=');
        if (eqPos == std::string::npos)
            continue;
        std::string name = token.substr(0, eqPos);
        char valChar = token[eqPos + 1];
        verilog_simulator::Logic val = (valChar == '1') ? verilog_simulator::Logic::ONE : verilog_simulator::Logic::ZERO;
        inputs[name] = val;
    }

    graph.setInputs(inputs);
    graph.evaluate();

    // Export DOT and JSON
    if (graph.saveDot("design.dot"))
    {
        std::cout << "Graph exported to design.dot (Graphviz)" << std::endl;
    }
    if (graph.saveJson("design.json"))
    {
        std::cout << "Graph exported to design.json (JSON)" << std::endl;
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

        // Print value if not X
        auto val = node->getValue();
        char vchar = verilog_simulator::to_char(val);
        std::cout << "  Value: " << vchar << std::endl;
    }

    return 0;
}