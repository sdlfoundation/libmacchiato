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

#include "Assembler.h"

#include "Patch/AssemblyHook.h"
#include "Patch/Data.h"
#include "Patch/Detour.h"
#include "Patch/Error.h"
#include "Patch/Hook.h"
#include "Patch/Line.h"
#include "Patch/Trampoline.h"

#include <cstdint>
#include <expected>
#include <memory>
#include <string>
#include <typeindex>
#include <variant>
#include <vector>

namespace LibMacchiato {
    typedef std::variant<DataPatch<u32>, DataPatch<u16>, DataPatch<u8>,
                         DataPatch<s32>, DataPatch<s16>, DataPatch<s8>,
                         DataPatch<f32>, LinePatch, TrampolinePatch,
                         DetourPatch, Hook, AssemblyHook>
        PatchComponent;

    struct Patch {
      private:
        Patch(bool enabled, std::vector<PatchComponent> components)
            : enabled(enabled)
            , components(components) {}

        bool                        enabled;
        std::vector<PatchComponent> components;

      public:
        static Patch create() noexcept { return Patch(false, {}); }

        std::vector<PatchComponent>& getComponents() {
            return this->components;
        }

        [[nodiscard]] inline Patch&&
        withLine(const uintptr_t   address,
                 const std::string instruction) && noexcept {
            std::expected<LinePatch, PatchError> maybeLine =
                LinePatch::line(address, instruction);

            if (!maybeLine.has_value()) {
                MERROR("Line \"{}\" error: {}", instruction,
                       patchErrorToStr(maybeLine.error()));
                return std::move(*this);
            }

            this->components.push_back(maybeLine.value());
            return std::move(*this);
        }

        template <typename T>
        [[nodiscard]] inline Patch&& withData(uintptr_t address,
                                              T         data) && noexcept {
            this->components.push_back(DataPatch<T>::create(address, data));
            return std::move(*this);
        }

        [[nodiscard]] inline Patch&&
        withLine(const LinePatch line) && noexcept {
            this->components.push_back(line);
            return std::move(*this);
        }

        [[nodiscard]] inline Patch&& withLine(
            const std::expected<LinePatch, PatchError> maybeLine) && noexcept {
            if (!maybeLine.has_value()) {
                MERROR("Failed to apply line: \"",
                       patchErrorToStr(maybeLine.error()), "\"");
                return std::move(*this);
            }

            this->components.push_back(maybeLine.value());
            return std::move(*this);
        }

        [[nodiscard]] inline Patch&&
        withTrampoline(const TrampolinePatch trampoline) && noexcept {
            this->components.push_back(trampoline);
            return std::move(*this);
        }

        [[nodiscard]] inline Patch&&
        withTrampolines(std::vector<TrampolinePatch> trampolines) && noexcept {
            for (const auto& trampoline : trampolines) {
                this->components.push_back(trampoline);
            }

            return std::move(*this);
        }

        [[nodiscard]] inline Patch&&
        withDetour(const DetourPatch detour) && noexcept {
            this->components.push_back(detour);
            return std::move(*this);
        }

        [[nodiscard]] inline Patch&&
        withHook(uintptr_t address, const void* function) && noexcept {
            this->components.push_back(Hook::create(address, function));
            return std::move(*this);
        }

        [[nodiscard]] inline Patch&& withHook(const Hook hook) && noexcept {
            this->components.push_back(hook);
            return std::move(*this);
        }

        [[nodiscard]] inline Patch&&
        withAssemblyHook(const AssemblyHook hook) && noexcept {
            this->components.push_back(hook);
            return std::move(*this);
        }

        [[nodiscard]] inline Patch&&
        withAssemblyHook(uintptr_t                address,
                         std::vector<std::string> instructions,
                         bool keepOriginalBytes) && noexcept {
            this->components.push_back(AssemblyHook::assemble(
                address, instructions, keepOriginalBytes));
            return std::move(*this);
        }

        inline void enable() {
            if (this->enabled) [[unlikely]]
                return;

            auto visitor = [](auto&& component) { component.enable(); };
            for (auto& component : this->components)
                std::visit(visitor, component);

            this->enabled = true;
        }

        inline void disable() {
            if (!this->enabled) [[unlikely]]
                return;

            auto visitor = [](auto&& component) { component.disable(); };
            for (auto& component : this->components)
                std::visit(visitor, component);

            this->enabled = false;
        }
    };

#define PATCH(name) [[nodiscard]] ::LibMacchiato::Patch name()
#define DECL_PATCH(name) [[nodiscard]] ::LibMacchiato::Patch name();
} // namespace LibMacchiato
