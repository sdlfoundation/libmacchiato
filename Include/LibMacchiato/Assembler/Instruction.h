#pragma once

#include "Mnemonic.h"
#include "Operand.h"

namespace LibMacchiato::PPCAssembler {
    struct PPCInstruction {
        PPCMnemonic mnemonic = {};
        OperandList operands = {};
    };
} // namespace LibMacchiato::PPCAssembler
