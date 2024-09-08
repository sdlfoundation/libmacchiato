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

#include "../Assembler/Mask.h"
#include "../Utils/Assembly.h"
#include "../Utils/Memory.h"
#include "Line.h"

#include <sdl-utils/Types.h>

#include <array>
#include <expected>
#include <format>
#include <functional>
#include <vector>

namespace LibMacchiato {
    struct Hook {
      private:
        Hook(std::vector<LinePatch> branch)
            : branch(branch) {}

        std::vector<LinePatch> branch;

      public:
        inline void enable() {
            for (auto& patch : this->branch) {
                patch.enable();
            }
        }

        inline void disable() {
            for (auto& patch : this->branch) {
                patch.disable();
            }
        }

        [[nodiscard]] static Hook create(uintptr_t   address,
                                         const void* function) {
            // Utils::Assembly::adjustAddressIfFirstInstructionIsBranch(address);

            auto customFunctionAddress = reinterpret_cast<uintptr_t>(function);

            std::vector<LinePatch> branch =
                Utils::Assembly::jump(address, customFunctionAddress);

            return Hook(std::move(branch));
        }

        [[nodiscard]] inline const std::vector<LinePatch>&
        getBranchData() const noexcept {
            return this->branch;
        }
    }; // namespace LibMacchiato
} // namespace LibMacchiato
