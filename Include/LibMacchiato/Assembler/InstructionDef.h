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
