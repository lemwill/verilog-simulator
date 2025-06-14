#include "uhdm_reader.hpp"
#include <uhdm/vpi_uhdm.h>
#include <iostream>

UHDMReader::UHDMReader() : design_(nullptr)
{
    serializer_ = std::make_unique<UHDM::Serializer>();
}

UHDMReader::~UHDMReader() = default;

bool UHDMReader::loadDatabase(const std::string &filename)
{
    try
    {
        // Restore returns a vector of vpiHandle, one per top-level design
        auto handles = serializer_->Restore(filename);
        if (handles.empty())
        {
            std::cerr << "Failed to load UHDM database: " << filename << std::endl;
            return false;
        }
        // Get the UHDM::design pointer from the first handle
        design_ = UhdmDesignFromVpiHandle(handles[0]);
        if (!design_)
        {
            std::cerr << "Failed to extract UHDM::design from handle." << std::endl;
            return false;
        }
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading UHDM database: " << e.what() << std::endl;
        return false;
    }
}

UHDM::design *UHDMReader::getDesign() const
{
    return design_;
}