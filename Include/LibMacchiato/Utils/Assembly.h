#pragma once

#include "../Patch/Line.h"

#include <sdl-utils/Types.h>

#include <vector>

namespace LibMacchiato::Utils::Assembly {
    uintptr_t getAdjustedAddressIfFirstInstructionIsBranch(uintptr_t address);

    inline bool shortJumpIsPossible(uintptr_t address) {
        return address <= 0x01FFFFFC;
    }

    inline size_t getJumpSize(uintptr_t address) {
        return shortJumpIsPossible(address) ? 1 : 4;
    }

    std::vector<u32>       jump(u32 dst);
    std::vector<LinePatch> jump(u32 address, u32 dst);
} // namespace LibMacchiato::Utils::Assembly
