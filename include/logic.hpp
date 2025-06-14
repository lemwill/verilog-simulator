#pragma once

#include <cstdint>

namespace verilog_simulator
{

    // 4-state logic trimmed to 3 for MVP (0,1,X)
    enum class Logic : uint8_t
    {
        ZERO,
        ONE,
        X
    };

    inline char to_char(Logic v)
    {
        switch (v)
        {
        case Logic::ZERO:
            return '0';
        case Logic::ONE:
            return '1';
        default:
            return 'X';
        }
    }

} // namespace verilog_simulator