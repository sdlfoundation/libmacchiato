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

#include <expected>
#include <sdl-utils/Types.h>
#include <vpad/input.h>

namespace LibMacchiato {
    struct VPADInput {
        bool      unused : 0x1;
        bool      StickLEmulationLeft : 0x1;
        bool      StickLEmulationRight : 0x1;
        bool      StickLEmulationUp : 0x1;
        bool      StickLEmulationDown : 0x1;
        bool      StickREmulationLeft : 0x1;
        bool      StickREmulationRight : 0x1;
        bool      StickREmulationUp : 0x1;
        bool      StickREmulationDown : 0x1;
        bool      unused2 : 0x4;
        bool      StickLButton : 0x1;
        bool      StickRButton : 0x1;
        bool      ButtonTV : 0x1;
        bool      ButtonA : 0x1;
        bool      ButtonB : 0x1;
        bool      ButtonX : 0x1;
        bool      ButtonY : 0x1;
        bool      ButtonLeft : 0x1;
        bool      ButtonRight : 0x1;
        bool      ButtonUp : 0x1;
        bool      ButtonDown : 0x1;
        bool      ButtonZL : 0x1;
        bool      ButtonZR : 0x1;
        bool      ButtonL : 0x1;
        bool      ButtonR : 0x1;
        bool      ButtonPlus : 0x1;
        bool      ButtonMinus : 0x1;
        bool      ButtonHome : 0x1;
        bool      ButtonSync : 0x1;
        VPADVec2D leftStick;

        [[nodiscard]] static std::expected<VPADInput, VPADReadError> read() {
            VPADStatus    vpadStatus = {};
            VPADReadError vpadError;

            VPADRead(VPAD_CHAN_0, &vpadStatus, 1, &vpadError);

            if (vpadError > 0) {
                return std::unexpected<VPADReadError>(vpadError);
            }

            // auto input = static_cast<VPADInput>(vpadStatus.hold);
            VPADInput* input = (VPADInput*)&vpadStatus.hold;

            input->leftStick = vpadStatus.leftStick;

            return *input;
        }
    };

    static_assert(sizeof(VPADInput) == sizeof(u32) + sizeof(VPADVec2D));
} // namespace LibMacchiato
