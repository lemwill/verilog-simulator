# Verilog Simulator - UHDM Reader

This project provides a foundation for a Verilog simulator by implementing a UHDM (Universal Hardware Data Model) database reader.

## Prerequisites

- CMake 3.14 or higher
- C++17 compatible compiler
- UHDM library installed on your system

## Building the Project

1. Create a build directory:

```bash
mkdir build
cd build
```

2. Configure and build:

```bash
cmake ..
make
```

## Usage

The program takes a UHDM database file as input:

```bash
./uhdm_reader path/to/your/database.uhdm
```

## Project Structure

- `src/` - Source files
  - `main.cpp` - Main program entry point
  - `uhdm_reader.cpp` - Implementation of the UHDM reader
- `include/` - Header files
  - `uhdm_reader.hpp` - UHDM reader class declaration
- `CMakeLists.txt` - Build configuration

## License

This project is open source and available under the MIT License.
