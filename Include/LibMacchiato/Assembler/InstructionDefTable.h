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

#include "InstructionDef.h"
#include "Mask.h"
#include "Mnemonic.h"
#include "OperandDef.h"

namespace LibMacchiato::PPCAssembler {
    constexpr OperandDefList D_FORM = {
        OperandDef{.pos  = 6,
                   .bits = 5,
                   .type = DirectOperandDefType{.regType = RegisterType::GPR}},
        OperandDef{.pos  = 11,
                   .bits = 5,
                   .type = DirectOperandDefType{.regType = RegisterType::GPR}},
        OperandDef{.pos = 16, .bits = 16, .type = ImmediateOperandDefType{}}};

    constexpr OperandDefList INDIRECT_D_FORM = {
        OperandDef{.pos  = 6,
                   .bits = 5,
                   .type = DirectOperandDefType{.regType = RegisterType::GPR}},
        OperandDef{
            .pos  = 11,
            .bits = 21,
            .type = IndirectOperandDefType{.offsetPos = 16, .offsetBits = 16}},
        std::nullopt};

    constexpr OperandDefList B_FORM = {
        OperandDef{.pos = 6, .bits = 26, .type = ImmediateOperandDefType{}},
        std::nullopt, std::nullopt};

    constexpr OperandDefList S_FORM = {
        OperandDef{.pos  = 6,
                   .bits = 5,
                   .type = DirectOperandDefType{.regType = RegisterType::GPR}},
        std::nullopt, std::nullopt};

    constexpr std::array<PPCInstructionDef, 20> PPC_INSTRUCTION_TABLE = {
        PPCInstructionDef{.mnemonic = PPCMnemonic::ADDIC,
                          .operands = D_FORM,
                          .mask     = 0},
        PPCInstructionDef{.mnemonic = PPCMnemonic::ADDI,
                          .operands = D_FORM,
                          .mask     = 0},
        PPCInstructionDef{.mnemonic = PPCMnemonic::ADDIS,
                          .operands = D_FORM,
                          .mask     = 0},
        PPCInstructionDef{
            .mnemonic = PPCMnemonic::LI,
            .operands = {OperandDef{.pos  = 6,
                                    .bits = 5,
                                    .type =
                                        DirectOperandDefType{
                                            .regType = RegisterType::GPR}},
                         OperandDef{.pos  = 16,
                                    .bits = 16,
                                    .type = ImmediateOperandDefType{}},
                         std::nullopt},
            .mask     = 0},
        PPCInstructionDef{
            .mnemonic = PPCMnemonic::LIS,
            .operands = {OperandDef{.pos  = 6,
                                    .bits = 5,
                                    .type =
                                        DirectOperandDefType{
                                            .regType = RegisterType::GPR}},
                         OperandDef{.pos  = 16,
                                    .bits = 16,
                                    .type = ImmediateOperandDefType{}},
                         std::nullopt},
            .mask     = 0},
        PPCInstructionDef{.mnemonic = PPCMnemonic::ORI,
                          .operands = D_FORM,
                          .mask     = 0},
        PPCInstructionDef{.mnemonic = PPCMnemonic::B,
                          .operands = B_FORM,
                          .mask     = 0},
        PPCInstructionDef{.mnemonic = PPCMnemonic::BA,
                          .operands = B_FORM,
                          .mask     = BIT_MASK_AA},
        PPCInstructionDef{.mnemonic = PPCMnemonic::BL,
                          .operands = B_FORM,
                          .mask     = BIT_MASK_LK},
        PPCInstructionDef{
            .mnemonic = PPCMnemonic::BLR,
            .operands = {std::nullopt, std::nullopt, std::nullopt},
            .mask     = BIT_MASK_LR},
        PPCInstructionDef{
            .mnemonic = PPCMnemonic::NOP,
            .operands = {std::nullopt, std::nullopt, std::nullopt},
            .mask     = 0},
        PPCInstructionDef{
            .mnemonic = PPCMnemonic::BCTR,
            .operands = {std::nullopt, std::nullopt, std::nullopt},
            .mask     = BIT_MASK_BI | BIT_MASK_BO | BIT_MASK_BCTR},
        PPCInstructionDef{
            .mnemonic = PPCMnemonic::MTSPR,
            .operands = {OperandDef{.pos  = 12,
                                    .bits = 10,
                                    .type = ImmediateOperandDefType{}},
                         OperandDef{.pos  = 6,
                                    .bits = 5,
                                    .type =
                                        DirectOperandDefType{
                                            .regType = RegisterType::GPR}},
                         std::nullopt},
            .mask     = BIT_MASK_MTSPR},
        PPCInstructionDef{
            .mnemonic = PPCMnemonic::MTCTR,
            .operands = {OperandDef{.pos  = 6,
                                    .bits = 5,
                                    .type =
                                        DirectOperandDefType{
                                            .regType = RegisterType::GPR}},
                         std::nullopt, std::nullopt},
            .mask     = BIT_MASK_MTCTR | BIT_MASK_MTSPR},
        PPCInstructionDef{
            .mnemonic = PPCMnemonic::STW,
            .operands = {OperandDef{.pos  = 6,
                                    .bits = 5,
                                    .type =
                                        DirectOperandDefType{
                                            .regType = RegisterType::GPR}},
                         std::nullopt, std::nullopt},
            .mask     = BIT_MASK_MTCTR | BIT_MASK_MTSPR},
        PPCInstructionDef{
            .mnemonic = PPCMnemonic::STWU,
            .operands = {OperandDef{.pos  = 6,
                                    .bits = 5,
                                    .type =
                                        DirectOperandDefType{
                                            .regType = RegisterType::GPR}},
                         std::nullopt, std::nullopt},
            .mask     = BIT_MASK_MTCTR | BIT_MASK_MTSPR},
        PPCInstructionDef{.mnemonic = PPCMnemonic::LWZ,
                          .operands = INDIRECT_D_FORM,
                          .mask     = BIT_MASK_MTCTR | BIT_MASK_MTSPR},
        PPCInstructionDef{.mnemonic = PPCMnemonic::LBZ,
                          .operands = INDIRECT_D_FORM,
                          .mask     = BIT_MASK_MTCTR | BIT_MASK_MTSPR},
        PPCInstructionDef{.mnemonic = PPCMnemonic::STW,
                          .operands = INDIRECT_D_FORM,
                          .mask     = BIT_MASK_MTCTR | BIT_MASK_MTSPR},
        PPCInstructionDef{.mnemonic = PPCMnemonic::STB,
                          .operands = INDIRECT_D_FORM,
                          .mask     = BIT_MASK_MTCTR | BIT_MASK_MTSPR}

    };
} // namespace LibMacchiato::PPCAssembler
