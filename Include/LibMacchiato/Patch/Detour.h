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

#include "../Assert.h"
#include "../Log.h"

#include "Hook.h"
#include "Line.h"

#include <concepts>
#include <cstdlib>
#include <cstring>
#include <expected>
#include <functional>
#include <optional>
#include <sdl-utils/Types.h>

namespace LibMacchiato {
    /*
     *
     * @brief A reduced version of a trampoline patch. Does the same thing as a
     * trampoline patch expect you do not have access to the original function
     * that is being replaced and it is not called. Saves memory from having
     * to copy the original function.
     *
     */
    struct DetourPatch {
      private:
        DetourPatch(Hook hook)
            : hook(hook) {}

        Hook hook;

      public:
        void enable() { this->hook.enable(); }
        void disable() { this->hook.disable(); }

        /*
         *
         * @warning The constructor assumes that the function passed is at least
         * 16 bytes long in order to install the hook. Passing a function
         * smaller than 16 bytes will lead to memory corruption.
         *
         */
        [[nodiscard]] static DetourPatch create(uintptr_t address,
                                                void*     function) {
            return DetourPatch(Hook::create(address, function));
        }
    };
} // namespace LibMacchiato
