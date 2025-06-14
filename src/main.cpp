#include "uhdm_reader.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <uhdm_file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    UHDMReader reader;

    if (!reader.loadDatabase(filename))
    {
        std::cerr << "Failed to load UHDM database" << std::endl;
        return 1;
    }

    // Get the design and print some basic information
    UHDM::design *design = reader.getDesign();
    if (design)
    {
        std::cout << "Successfully loaded UHDM database" << std::endl;
        std::cout << "Number of modules: " << design->AllModules()->size() << std::endl;
    }

    return 0;
}