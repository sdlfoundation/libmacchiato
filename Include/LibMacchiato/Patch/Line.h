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

#include "../Assembler.h"
#include "../Log.h"
#include "../Utils/Memory.h"
#include "Error.h"

#include <sdl-utils/Types.h>

#include <coreinit/cache.h>
#include <coreinit/memorymap.h>

#include <cstdint>
#include <expected>
#include <format>
#include <memory>
#include <string>
#include <variant>
#include <vector>

// Ensure that the compilation target is 32-bit.
static_assert(sizeof(uintptr_t) == sizeof(u32));

namespace LibMacchiato {
    enum class BranchType {
        Branch,
        BranchLink,
        BranchIfEqual,
        BranchIfNotEqual,
    };

    namespace {
        inline std::string branchTypeToString(const BranchType branchType) {
            switch (branchType) {
            case BranchType::Branch:
                return "b";
            case BranchType::BranchLink:
                return "bl";
            case BranchType::BranchIfEqual:
                return "beq";
            case BranchType::BranchIfNotEqual:
                return "bne";
            }

            return "unk";
        }
    } // namespace

    struct LinePatch {
      private:
        LinePatch(uintptr_t address, uintptr_t enableAssembly,
                  std::optional<uintptr_t> disableAssembly)
            : address(address)
            , enableAssembly(enableAssembly)
            , disableAssembly(disableAssembly) {}

        inline void apply(bool enable) {
            u32 address = reinterpret_cast<u32>(this->address);

            if (!this->disableAssembly.has_value()) [[unlikely]] {
                this->disableAssembly = Utils::Memory::readU32(address);
            }

            u32 assembly;

            enable ? assembly = static_cast<u32>(this->enableAssembly)
                   : assembly = static_cast<u32>(this->disableAssembly.value());

            Utils::Memory::writeInstruction(address, assembly);
        }

        // TODO:
        // ~LinePatch() {
        //     // Avoid corrupting the memory by forgetting to disable
        //     // a patch before it gets deleted.
        //     this->disable();
        // }

        uintptr_t address;
        uintptr_t enableAssembly;

        // The original instruction may not be loaded at the time of the
        // construction of a `LinePatch`, so it gets fetched lazily.
        std::optional<uintptr_t> disableAssembly;

      public:
        void enable() { this->apply(true); }
        void disable() { this->apply(false); }

        [[nodiscard]] static LinePatch create(uintptr_t address,
                                              uintptr_t enableAssembly) {
            return LinePatch(address, enableAssembly, std::nullopt);
        }

        [[nodiscard]] static std::expected<LinePatch, PatchError>
        line(uintptr_t address, const std::string instruction) {
            std::expected<u32, PPCAssembler::AssembleError> assembledCode =
                PPCAssembler::assemble(instruction);

            if (!assembledCode.has_value()) {
                return std::unexpected(assembledCode.error());
            }

            return LinePatch::create(address, assembledCode.value());
        }

        [[nodiscard]] static std::expected<std::vector<LinePatch>, PatchError>
        multiline(const uintptr_t                address,
                  const std::vector<std::string> instructions) {
            std::vector<LinePatch> lines;

            size_t i = 0;
            for (const auto& instruction : instructions) {
                std::expected<LinePatch, PatchError> line =
                    LinePatch::line(address + sizeof(u32) * i, instruction);

                if (!line.has_value())
                    return std::unexpected<PatchError>(line.error());

                lines.push_back(line.value());

                i++;
            }

            return lines;
        }

        [[nodiscard]] static std::expected<LinePatch, PatchError>
        shortBranch(BranchType branchType, uintptr_t address, void* function) {
            auto      functionAddress = reinterpret_cast<uintptr_t>(function);
            uintptr_t relativeAddress = functionAddress - address;

            std::string branchTypeStr = branchTypeToString(branchType);
            std::string instruction =
                std::format("{} {}", branchTypeStr, relativeAddress);

            return LinePatch::line(address, instruction);
        }

        template <typename Class, typename Return, typename... Args>
        [[nodiscard]] static std::expected<LinePatch, PatchError>
        shortBranch(BranchType branchType, uintptr_t address,
                    Return (Class::*function)(Args...)) {
            return LinePatch::shortBranch(branchType, address,
                                          reinterpret_cast<void*>(function));
        }

        [[nodiscard]] uintptr_t getAddress() const noexcept {
            return this->address;
        }

        [[nodiscard]] uintptr_t getDisableAssembly() const noexcept {
            return this->disableAssembly.has_value()
                       ? this->disableAssembly.value()
                       : reinterpret_cast<uintptr_t>(Utils::Memory::readU32(
                           reinterpret_cast<u32>(this->address)));
        }
    };
} // namespace LibMacchiato
