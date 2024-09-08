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

#include "Assembler/Error.h"
#include "Assembler/Instruction.h"
#include "Assembler/InstructionDef.h"
#include "Assembler/InstructionDefTable.h"
#include "Assembler/Mask.h"
#include "Assembler/Operand.h"
#include "Assembler/OperandDef.h"
#include "Assembler/PPCData.h"

#include <sdl-utils/Types.h>

#include <cstddef>
#include <expected>
#include <string>
#include <variant>

namespace LibMacchiato::PPCAssembler {
    [[nodiscard]] std::expected<u32, AssembleError>
    assemble(const std::string& instructionStr);
}
