#pragma once

#include <string>
#include <memory>
#include <uhdm/uhdm.h>
#include <uhdm/vpi_uhdm.h>

class UHDMReader
{
public:
    UHDMReader();
    ~UHDMReader();

    // Load a UHDM database from a file
    bool loadDatabase(const std::string &filename);

    // Get the design (top-level module)
    UHDM::design *getDesign() const;

private:
    std::unique_ptr<UHDM::Serializer> serializer_;
    UHDM::design *design_;
};