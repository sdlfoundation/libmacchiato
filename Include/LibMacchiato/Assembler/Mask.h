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

#pragma once

#include "PPCData.h"

#include <sdl-utils/Types.h>

#include <cstdint>

namespace LibMacchiato::PPCAssembler {
    constexpr u32 BIT_MASK_AA   = 0b00000000000000000000000000000010;
    constexpr u32 BIT_MASK_LK   = 0b00000000000000000000000000000001;
    constexpr u32 BIT_MASK_LR   = 0b00000010100000000000000000100000;
    constexpr u32 BIT_MASK_BI   = 0b00000010100000000000000000000000;
    constexpr u32 BIT_MASK_BO   = 0b00000000000000000000000000000000;
    constexpr u32 BIT_MASK_SP   = 0b00000000000000000000000000000000;
    constexpr u32 BIT_MASK_SPR1 = 0b00000000000000000000000000000000;

    constexpr u32 BIT_MASK_BCTR  = 0b00000000000000000000010000100000;
    constexpr u32 BIT_MASK_MTSPR = 0b00000000000000000000001110100110;
    constexpr u32 BIT_MASK_MTCTR = 0b00000000000010010000000000000000;

    constexpr u32 BIT_MASK_OPCODE = (1 << OPCODE_SIZE) - 1;


    constexpr u32 BIT_MASK_SHORT_JUMP = 0x01FFFFFC;
} // namespace LibMacchiato::PPCAssembler
