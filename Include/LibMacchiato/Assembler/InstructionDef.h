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

#include "Mnemonic.h"
#include "Operand.h"
#include "OperandDef.h"

#include <sdl-utils/Types.h>

namespace LibMacchiato::PPCAssembler {
    struct PPCInstructionDef {
        PPCMnemonic    mnemonic = {};
        OperandDefList operands = {};
        u32            mask     = 0;

        constexpr bool isEqWithOperands(const OperandList& cmpOperands) const {
            for (size_t i = 0; i < OPERAND_NUM; i++) {
                if (this->operands[i].has_value()
                    && cmpOperands[i].has_value()) {
                    if (!operandIsEqWithDef(cmpOperands[i].value(),
                                            this->operands[i].value()))
                        return false;
                } else if (this->operands[i].has_value()
                           || cmpOperands[i].has_value())
                    return false;
            }

            return true;
        }
    };
} // namespace LibMacchiato::PPCAssembler
