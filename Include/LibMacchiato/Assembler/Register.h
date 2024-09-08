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

#include <sdl-utils/Types.h>

#include <cstdint>
#include <optional>

namespace LibMacchiato::PPCAssembler {
    enum class RegisterType {
        GPR,
        FPR,
    };

    enum class Register {
        R0,
        R1,
        R2,
        R3,
        R4,
        R5,
        R6,
        R7,
        R8,
        R9,
        R10,
        R11,
        R12,
        R13,
        R14,
        R15,
        R16,
        R17,
        R18,
        R19,
        R20,
        R21,
        R22,
        R23,
        R24,
        R25,
        R26,
        R27,
        R28,
        R29,
        R30,
        R31,
        R32,
    };

    inline std::optional<Register> numToReg(u32 reg) {
        switch (reg) {
        case 0:
            return Register::R0;
        case 1:
            return Register::R1;
        case 2:
            return Register::R2;
        case 3:
            return Register::R3;
        case 4:
            return Register::R4;
        case 5:
            return Register::R5;
        case 6:
            return Register::R6;
        case 7:
            return Register::R7;
        case 8:
            return Register::R8;
        case 9:
            return Register::R9;
        case 10:
            return Register::R10;
        case 11:
            return Register::R11;
        case 12:
            return Register::R12;
        case 13:
            return Register::R13;
        case 14:
            return Register::R14;
        case 15:
            return Register::R15;
        case 16:
            return Register::R16;
        case 17:
            return Register::R17;
        case 18:
            return Register::R18;
        case 19:
            return Register::R19;
        case 20:
            return Register::R20;
        case 21:
            return Register::R21;
        case 22:
            return Register::R22;
        case 23:
            return Register::R23;
        case 24:
            return Register::R24;
        case 25:
            return Register::R25;
        case 26:
            return Register::R26;
        case 27:
            return Register::R27;
        case 28:
            return Register::R28;
        case 29:
            return Register::R29;
        case 30:
            return Register::R30;
        case 31:
            return Register::R31;
        case 32:
            return Register::R32;
        default:
            return std::nullopt;
        }
    }

    inline u32 regToNum(Register reg) {
        switch (reg) {
        case Register::R0:
            return 0;
        case Register::R1:
            return 1;
        case Register::R2:
            return 2;
        case Register::R3:
            return 3;
        case Register::R4:
            return 4;
        case Register::R5:
            return 5;
        case Register::R6:
            return 6;
        case Register::R7:
            return 7;
        case Register::R8:
            return 8;
        case Register::R9:
            return 9;
        case Register::R10:
            return 10;
        case Register::R11:
            return 11;
        case Register::R12:
            return 12;
        case Register::R13:
            return 13;
        case Register::R14:
            return 14;
        case Register::R15:
            return 15;
        case Register::R16:
            return 16;
        case Register::R17:
            return 17;
        case Register::R18:
            return 18;
        case Register::R19:
            return 19;
        case Register::R20:
            return 20;
        case Register::R21:
            return 21;
        case Register::R22:
            return 22;
        case Register::R23:
            return 23;
        case Register::R24:
            return 24;
        case Register::R25:
            return 25;
        case Register::R26:
            return 26;
        case Register::R27:
            return 27;
        case Register::R28:
            return 28;
        case Register::R29:
            return 29;
        case Register::R30:
            return 30;
        case Register::R31:
            return 31;
        case Register::R32:
            return 32;
        default:
            return 0;
        }
    }
} // namespace LibMacchiato::PPCAssembler
