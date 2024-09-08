/*
 * libmacchiato - Front-end for the Macchiato modding environment
 * Copyright (C) 2024 splatoon1enjoyer @ SDL Foundation
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "LibMacchiato/Utils/Assembly.h"
#include "LibMacchiato/Assembler.h"
#include "LibMacchiato/Utils/Memory.h"

#include <format>
#include <optional>

namespace LibMacchiato::Utils::Assembly {
    uintptr_t getAdjustedAddressIfFirstInstructionIsBranch(uintptr_t address) {
        u32 addressData   = Utils::Memory::readU32(static_cast<u32>(address));
        u32 addressOpcode = addressData & 0xFC000000;

        bool addressOpcodeIsBranch = (addressOpcode & 0x48000000) == 0x48000000;

        if (!addressOpcodeIsBranch)
            return address;

        u32 aa         = (addressData & 0x00000002) >> 1;
        u32 lk         = addressData & 0x00000001;
        u32 jumpOffset = addressData & 0x03FFFFFC;

        bool branchUsesAbsoluteAddress = aa == 1 && lk == 0;

        if (branchUsesAbsoluteAddress) {
            address = jumpOffset >> 2;
        } else {
            // bool jumpOffsetIsNegative = (jumpOffset & 0x02000000) ==
            // 0x02000000;
            bool jumpOffsetIsNegative = (jumpOffset & 0x02000000) != jumpOffset;

            if (jumpOffsetIsNegative) {
                jumpOffset += 0xFE000000;
            }

            address += jumpOffset;
        }

        return getAdjustedAddressIfFirstInstructionIsBranch(address);
    }

    std::optional<u32> shortJump(u32 dst) {
        if (dst > 0x01FFFFFC)
            return std::nullopt;

        return PPCAssembler::assemble(std::format("ba {}", dst)).value();
    }

    std::vector<u32> longJump(u32 dst) {
        std::pair<u16, u16> jumpAddressHalfs =
            Utils::Memory::splitAddress(static_cast<uintptr_t>(dst));

        std::vector<u32> jump = {
            PPCAssembler::assemble(
                std::format("lis r11, {}",
                            std::to_string(jumpAddressHalfs.first)))
                .value(),

            PPCAssembler::assemble("ori r11, r11, "
                                   + std::to_string(jumpAddressHalfs.second))
                .value(),
            PPCAssembler::assemble("mtctr r11").value(),
            PPCAssembler::assemble("bctr").value()};

        return jump;
    }

    std::vector<u32> jump(u32 dst) {
        std::optional<u32> maybeShortJump = shortJump(dst);

        if (maybeShortJump.has_value())
            return {maybeShortJump.value()};

        return longJump(dst);
    }

    std::vector<LinePatch> jump(u32 address, u32 dst) {
        std::vector<u32>       jumpBytes = jump(dst);
        std::vector<LinePatch> result    = {};

        u32 offset = 0;

        for (const auto byte : jumpBytes) {
            result.push_back(LinePatch::create(address + offset, byte));

            offset += sizeof(u32);
        }

        return result;
    }
} // namespace LibMacchiato::Utils::Assembly
