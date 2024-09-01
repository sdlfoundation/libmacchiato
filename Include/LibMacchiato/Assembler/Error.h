#pragma once

#include "Operand.h"
#include "OperandDef.h"
#include "Register.h"

#include <sdl-utils/Types.h>

#include <string>
#include <variant>

namespace LibMacchiato::PPCAssembler {
    struct InvalidInstruction {};

    struct EmptyInstruction {};

    struct InstructionTooLarge {};

    struct InvalidMnemonic {
        std::string mnemonic;
    };

    struct InvalidOperand {
        std::string operand;
    };

    struct OperandMismatch {
        Operand    operand;
        OperandDef expectedOperand;
    };

    struct IntegerTooLarge {
        u32    integer;
        size_t maxBits;
    };

    struct RegisterTypeMismatch {
        RegisterType regType;
        RegisterType expectedRegType;
    };

    struct IntegerConversionFailure {
        std::string integer;
    };

    typedef std::variant<IntegerConversionFailure, InvalidInstruction,
                         IntegerTooLarge, RegisterTypeMismatch, OperandMismatch,
                         EmptyInstruction, InstructionTooLarge, InvalidMnemonic,
                         InvalidOperand>
        AssembleError;

    inline std::string assembleErrorToStr(AssembleError error) {
        if (auto expectError = std::get_if<EmptyInstruction>(&error)) {
            return "instruction is empty";
        } else if (auto expectError =
                       std::get_if<InstructionTooLarge>(&error)) {
            return "instruction too large";
        } else if (auto expectError = std::get_if<InvalidMnemonic>(&error)) {
            return "invalid mnemonic";
        } else if (auto expectError = std::get_if<InvalidOperand>(&error)) {
            return "invalid operand";
        } else if (auto expectError = std::get_if<OperandMismatch>(&error)) {
            return "operand mismatch";
        } else if (auto expectError = std::get_if<IntegerTooLarge>(&error)) {
            return "integer \"" + std::to_string(expectError->integer)
                   + "\" too large";
        } else if (auto expectError =
                       std::get_if<RegisterTypeMismatch>(&error)) {
            return "register type mismatch";
        } else if (auto expectError = std::get_if<InvalidInstruction>(&error)) {
            return "invalid instruction";
        } else if (auto expectError =
                       std::get_if<IntegerConversionFailure>(&error)) {
            return "integer conversion for \"" + expectError->integer
                   + "\" failed";
        } else {
            return "unknown error";
        }

        return "unknown error";
    }
} // namespace LibMacchiato::PPCAssembler
