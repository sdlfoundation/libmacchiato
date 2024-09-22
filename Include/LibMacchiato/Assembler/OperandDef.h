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
#include "Register.h"

#include <array>
#include <optional>
#include <variant>

namespace LibMacchiato::PPCAssembler {
    struct DirectOperandDefType {
        RegisterType regType = RegisterType::GPR;
    };

    struct IndirectOperandDefType {
        size_t offsetPos  = 0;
        size_t offsetBits = 0;
    };

    struct ImmediateOperandDefType {};

    typedef std::variant<DirectOperandDefType, IndirectOperandDefType,
                         ImmediateOperandDefType>
        OperandDefType;

    struct OperandDef {
        size_t         pos  = 0;
        size_t         bits = 0;
        OperandDefType type = ImmediateOperandDefType{};
    };

    typedef std::array<std::optional<OperandDef>, OPERAND_NUM> OperandDefList;
} // namespace LibMacchiato::PPCAssembler
