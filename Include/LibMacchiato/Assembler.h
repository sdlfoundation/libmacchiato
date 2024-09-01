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
