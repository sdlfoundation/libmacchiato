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
