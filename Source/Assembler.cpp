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

#include "LibMacchiato/Assembler.h"
#include "LibMacchiato/Assembler/Error.h"

#include <sdl-utils/NumUtil.h>
#include <sdl-utils/StringUtil.h>
#include <sdl-utils/Types.h>

#include <cstddef>
#include <expected>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

//
#include "LibMacchiato/Log.h"

namespace LibMacchiato::PPCAssembler {
    namespace {
        // Generated assembly gets cached because modules tend to use similar
        // assemblies. Disable this functionality via:
        // `MACCHIATO_NO_ASSEMBLY_MEMO=1`.
        std::unordered_map<std::string, u32> assemblyMemo = {};
    } // namespace

    uint32_t generateMask(size_t bitWidth) {
        return bitWidth == 32 ? 0xFFFFFFFF
                              : (1U << static_cast<u32>(bitWidth)) - 1;
    }

    std::string format(const std::string& instruction) {
        std::string result = instruction;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    std::expected<Operand, AssembleError>
    tokenizeOperandSegment(const std::string_view segment) {
        std::string segmentStr = std::string(segment);

        if (segment.starts_with("0x")) {
            std::optional<u32> immediate =
                SDLUtils::hexStrToInt<u32>(segmentStr);
            if (!immediate.has_value())
                return std::unexpected<IntegerConversionFailure>(segmentStr);

            return ImmediateOperand{.value = immediate.value()};
        } else if (segment.starts_with("r")) {
            segmentStr.erase(0, 1);

            std::optional<u32> registerNum =
                SDLUtils::strToInt<u32>(segmentStr);
            if (!registerNum.has_value())
                return std::unexpected<IntegerConversionFailure>(segmentStr);

            std::optional<Register> reg = numToReg(registerNum.value());
            if (!reg.has_value())
                return std::unexpected<InvalidOperand>(
                    InvalidOperand{.operand = segmentStr});

            return DirectOperand{.regType = RegisterType::GPR,
                                 .reg     = reg.value()};
        } else if (segment.contains("(")) {
            if (!segment.contains(")")) {
                return std::unexpected<InvalidInstruction>(
                    InvalidInstruction{});
            }

            std::vector<std::string> indirectOperands =
                SDLUtils::splitf(std::string(segment), '(');

            std::string offsetStr       = indirectOperands[0];
            std::string rightStr        = indirectOperands[1];
            std::string baseRegisterStr = SDLUtils::splitf(rightStr, ')')[0];

            if (!std::string_view(baseRegisterStr).starts_with("r")) {
                return std::unexpected<InvalidOperand>(
                    InvalidOperand{.operand = baseRegisterStr});
            }

            baseRegisterStr.erase(0, 1);

            std::optional<u32> offset = std::nullopt;

            if (std::string_view(offsetStr).starts_with("0x")) {
                offset = SDLUtils::hexStrToInt<u32>(segmentStr);
            } else {
                offset = SDLUtils::strToInt<u32>(offsetStr);
            }

            if (!offset.has_value())
                return std::unexpected<IntegerConversionFailure>(segmentStr);

            std::optional<u32> baseRegister =
                SDLUtils::strToInt<u32>(baseRegisterStr);
            if (!baseRegister.has_value())
                return std::unexpected<IntegerConversionFailure>(
                    baseRegisterStr);

            std::optional<Register> reg = numToReg(baseRegister.value());
            if (!reg.has_value())
                return std::unexpected<InvalidOperand>(
                    InvalidOperand{.operand = baseRegisterStr});

            return IndirectOperand{.offset = offset.value(),
                                   .reg    = reg.value()};
        }

        std::optional<u32> immediate = SDLUtils::strToInt<u32>(segmentStr);
        if (!immediate.has_value())
            return std::unexpected<IntegerConversionFailure>(segmentStr);

        return ImmediateOperand{.value = immediate.value()};
    }

    /// Generates the operands from the given input.
    std::expected<OperandList, AssembleError>
    tokenizeOperandsSegment(const std::string& operandSegment) {
        std::vector<std::string> rawOperands =
            SDLUtils::splitf(operandSegment, ',');
        OperandList operands = {std::nullopt, std::nullopt, std::nullopt};

        size_t i = 0;

        for (auto rawOperand = rawOperands.begin();
             rawOperand != rawOperands.end(); rawOperand++) {
            size_t i = std::distance(rawOperands.begin(), rawOperand);

            std::expected<Operand, AssembleError> operand =
                tokenizeOperandSegment(*rawOperand);

            if (!operand.has_value()) {
                return std::unexpected<AssembleError>(operand.error());
            }

            operands[i] = operand.value();

            i++;
        }

        return operands;
    }

    std::expected<PPCInstruction, AssembleError>
    getInstruction(const std::string& instruction) {
        std::string trimmedInstruction = SDLUtils::trim(instruction);

        std::vector<std::string> instructionSegments =
            SDLUtils::splitf(trimmedInstruction, ' ');

        if (instructionSegments.empty()) {
            return std::unexpected<EmptyInstruction>(EmptyInstruction{});
        }

        std::string& mnemonicStr = instructionSegments[0];

        std::optional<PPCMnemonic> mnemonic = strToMnemonic(mnemonicStr);

        if (!mnemonic.has_value())
            return std::unexpected<InvalidMnemonic>(
                InvalidMnemonic{.mnemonic = mnemonicStr});

        std::vector<std::string> operandSegment(instructionSegments.begin() + 1,
                                                instructionSegments.end());

        std::string operandSegmentStr = {};
        for (const auto& operand : operandSegment) {
            operandSegmentStr = operandSegmentStr + operand;
        }

        if (!operandSegmentStr.empty()) {
            std::expected<OperandList, AssembleError> operands =
                tokenizeOperandsSegment(operandSegmentStr);

            if (!operands.has_value()) {
                return std::unexpected<AssembleError>(operands.error());
            }

            return PPCInstruction{.mnemonic = mnemonic.value(),
                                  .operands = operands.value()};
        } else {
            return PPCInstruction{.mnemonic = mnemonic.value(),
                                  .operands = EMPTY_OPERAND_LIST};
        }
    }

    /// Asserts that the input tokens match an instruction definition.
    std::optional<PPCInstructionDef>
    instructionize(const PPCMnemonic mnemonic, const OperandList& operands) {
        for (const auto& instructionDef : PPC_INSTRUCTION_TABLE) {
            if (instructionDef.mnemonic == mnemonic) {
                if (instructionDef.isEqWithOperands(operands)) {
                    return instructionDef;
                } else {
                    break;
                }
            }
        }

        return std::nullopt;
    }

    std::expected<u32, AssembleError>
    codegenSegment(const Operand& operand, const OperandDef& operandDef) {
        size_t pos  = operandDef.pos;
        size_t bits = operandDef.bits;

        if (auto immediate = std::get_if<ImmediateOperand>(&operand)) {
            if (!std::holds_alternative<ImmediateOperandDefType>(
                    operandDef.type)) {
                return std::unexpected<OperandMismatch>(OperandMismatch{
                    .operand = operand, .expectedOperand = operandDef});
            }

            size_t bitsTaken = SDLUtils::countBitsTaken(immediate->value);

            if (bitsTaken > bits) {
                return std::unexpected<IntegerTooLarge>(IntegerTooLarge{
                    .integer = immediate->value, .maxBits = bits});
            }

            return immediate->value << (32 - (pos + bits));
        } else if (auto reg = std::get_if<DirectOperand>(&operand)) {
            if (auto regDef =
                    std::get_if<DirectOperandDefType>(&operandDef.type)) {
                if (reg->regType != regDef->regType)
                    return std::unexpected<RegisterTypeMismatch>(
                        RegisterTypeMismatch{.regType = reg->regType,
                                             .expectedRegType =
                                                 regDef->regType});

                u32    regNum    = regToNum(reg->reg);
                size_t bitsTaken = SDLUtils::countBitsTaken(regNum);

                if (bitsTaken > bits) {
                    return std::unexpected<IntegerTooLarge>(IntegerTooLarge{
                        .integer = immediate->value, .maxBits = bits});
                }

                return regNum << (32 - (pos + bits));
            } else {
                return std::unexpected<OperandMismatch>(OperandMismatch{
                    .operand = operand, .expectedOperand = operandDef});
            }
        } else if (auto indirect = std::get_if<IndirectOperand>(&operand)) {
            if (auto indirectDef =
                    std::get_if<IndirectOperandDefType>(&operandDef.type)) {
                u32 value = 0;

                u32    base     = regToNum(indirect->reg);
                size_t basePos  = pos;
                size_t baseBits = bits - indirectDef->offsetBits;

                u32    offset     = indirect->offset;
                size_t offsetPos  = indirectDef->offsetPos;
                size_t offsetBits = indirectDef->offsetBits;

                if (SDLUtils::countBitsTaken(base) > baseBits) {
                    return std::unexpected<IntegerTooLarge>(
                        IntegerTooLarge{.integer = base, .maxBits = baseBits});
                }

                if (SDLUtils::countBitsTaken(offset) > offsetBits) {
                    return std::unexpected<IntegerTooLarge>(IntegerTooLarge{
                        .integer = offset, .maxBits = offsetBits});
                }

                value |= base << (basePos - baseBits);

                return value;
            } else {
                return std::unexpected<OperandMismatch>(OperandMismatch{
                    .operand = operand, .expectedOperand = operandDef});
            }
        } else {
            return std::unexpected<InvalidInstruction>(InvalidInstruction{});
        }
    }

    // Generates the final assembly from the tokens.
    std::expected<u32, AssembleError>
    codegen(const PPCInstruction&    instruction,
            const PPCInstructionDef& instructionDef) {
        u32 data = 0;

        data = (mnemonicToOpcode(instruction.mnemonic) & 0x3F) << 26;
        data |= instructionDef.mask;

        for (size_t i = 0; i < OPERAND_NUM; i++) {
            if (instruction.operands[i].has_value()
                && instructionDef.operands[i].has_value()) {
                std::expected<u32, AssembleError> result =
                    codegenSegment(instruction.operands[i].value(),
                                   instructionDef.operands[i].value());

                if (!result.has_value()) {
                    return std::unexpected<AssembleError>(result.error());
                }

                data |= result.value();
            }
        }

        return data;
    }

    std::expected<u32, AssembleError>
    assemble(const std::string& instructionStr) {
        const std::string formattedInstruction = format(instructionStr);

#ifndef MACCHIATO_NO_ASSEMBLY_MEMO
        if (assemblyMemo.contains(formattedInstruction))
            return assemblyMemo[formattedInstruction];
#endif

        const std::expected<PPCInstruction, AssembleError> instruction =
            getInstruction(formattedInstruction);
        if (!instruction.has_value())
            return std::unexpected<AssembleError>(instruction.error());

        std::optional<PPCInstructionDef> instructionDef = instructionize(
            instruction.value().mnemonic, instruction.value().operands);
        if (!instructionDef.has_value())
            return std::unexpected<InvalidInstruction>(InvalidInstruction{});

        std::expected<u32, AssembleError> code =
            codegen(instruction.value(), instructionDef.value());

        if (!code.has_value()) {
            return std::unexpected<AssembleError>(code.error());
        }

#ifndef MACCHIATO_NO_ASSEMBLY_MEMO
        assemblyMemo[formattedInstruction] = code.value();
#endif

        return code;
    }
} // namespace LibMacchiato::PPCAssembler
