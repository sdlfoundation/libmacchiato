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

#include "OperandDef.h"
#include "PPCData.h"

#include <sdl-utils/Types.h>

#include <array>
#include <cstdint>
#include <optional>
#include <variant>

namespace LibMacchiato::PPCAssembler {
    struct ImmediateOperand {
        u32 value = 0;
    };

    struct DirectOperand {
        RegisterType regType = RegisterType::GPR;
        Register     reg     = Register::R0;
    };

    struct IndirectOperand {
        u32      offset = 0;
        Register reg    = Register::R0;
    };

    typedef std::variant<ImmediateOperand, DirectOperand, IndirectOperand>
                                                            Operand;
    typedef std::array<std::optional<Operand>, OPERAND_NUM> OperandList;

    constexpr OperandList EMPTY_OPERAND_LIST = {std::nullopt, std::nullopt,
                                                std::nullopt};

    constexpr bool operandIsEqWithDef(const Operand&    operand,
                                      const OperandDef& operandDef) {
        if (std::holds_alternative<ImmediateOperand>(operand)
            && !std::holds_alternative<ImmediateOperandDefType>(
                operandDef.type))
            return false;
        if (std::holds_alternative<DirectOperand>(operand)
            && !std::holds_alternative<DirectOperandDefType>(operandDef.type))
            return false;
        if (std::holds_alternative<IndirectOperand>(operand)
            && !std::holds_alternative<IndirectOperandDefType>(operandDef.type))
            return false;

        return true;
    }
} // namespace LibMacchiato::PPCAssembler
