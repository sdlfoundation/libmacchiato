#pragma once

#include "../Patch/Line.h"

#include <sdl-utils/Types.h>

#include <vector>

namespace LibMacchiato::Utils::Assembly {
    uintptr_t getAdjustedAddressIfFirstInstructionIsBranch(uintptr_t address);

    inline bool isBranch(uint32_t instruction) {
        uint32_t opcode = (instruction >> 26) & 0x3F;

        return (opcode == 0x12) || (opcode == 0x10) || (opcode == 0x11);
    }

    // Function to sign-extend a 16-bit value to 32-bit
    inline int32_t sign_extend_16_to_32(uint32_t value) {
        if (value & 0x8000) { // Check if the 16th bit is set (negative number)
            return value | 0xFFFF0000; // Sign-extend by setting the higher bits
        } else {
            return value & 0x0000FFFF; // Zero-extend the value
        }
    }

    // Function to get the relative offset of a conditional branch instruction
    inline int32_t get_branch_offset(uint32_t instruction) {
        // Extract the most significant 6 bits (opcode)
        uint32_t opcode = (instruction >> 26) & 0x3F;

        // Check if the opcode is a conditional branch (`bc`)
        if (opcode != 0x10) {
            std::cerr << "Error: Instruction is not a conditional branch (bc)."
                      << std::endl;
            return 0;
        }

        // Extract the 16-bit signed offset (lower 16 bits of the instruction)
        uint32_t offset = instruction & 0xFFFF; // Mask to get 16-bit offset

        // Sign-extend the 16-bit offset to 32 bits
        int32_t signed_offset = sign_extend_16_to_32(offset);

        // Return the relative offset
        return signed_offset;
    }

    inline bool shortJumpIsPossible(uintptr_t address) {
        return address <= 0x01FFFFFC;
    }

    inline size_t getJumpSize(uintptr_t address) {
        return shortJumpIsPossible(address) ? 1 : 4;
    }

    std::vector<u32>       jump(u32 dst);
    std::vector<LinePatch> jump(u32 address, u32 dst);
} // namespace LibMacchiato::Utils::Assembly
