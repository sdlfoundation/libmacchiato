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

        [[nodiscard]] static VPADInput& getLatestRead() {
            static VPADInput instance;
            return instance;
        }

        [[nodiscard]] static std::expected<VPADInput, VPADReadError> read() {
            VPADStatus    vpadStatus = {};
            VPADReadError vpadError;

            VPADRead(VPAD_CHAN_0, &vpadStatus, 1, &vpadError);

            if (vpadError > 0) {
                return std::unexpected<VPADReadError>(vpadError);
            }

            VPADInput* input = (VPADInput*)&vpadStatus.hold;

            input->leftStick = vpadStatus.leftStick;

            getLatestRead() = *input;

            return *input;
        }
    };

    static_assert(sizeof(VPADInput) == sizeof(u32) + sizeof(VPADVec2D));

    inline void copyHoldToVPADInput(const VPADStatus& status,
                                    VPADInput&        input) {
        uint32_t hold = status.hold;

        input.StickLEmulationLeft  = (hold >> 0) & 0x1;
        input.StickLEmulationRight = (hold >> 1) & 0x1;
        input.StickLEmulationUp    = (hold >> 2) & 0x1;
        input.StickLEmulationDown  = (hold >> 3) & 0x1;
        input.StickREmulationLeft  = (hold >> 4) & 0x1;
        input.StickREmulationRight = (hold >> 5) & 0x1;
        input.StickREmulationUp    = (hold >> 6) & 0x1;
        input.StickREmulationDown  = (hold >> 7) & 0x1;

        input.StickLButton = (hold >> 12) & 0x1;
        input.StickRButton = (hold >> 13) & 0x1;
        input.ButtonTV     = (hold >> 14) & 0x1;
        input.ButtonA      = (hold >> 15) & 0x1;
        input.ButtonB      = (hold >> 16) & 0x1;
        input.ButtonX      = (hold >> 17) & 0x1;
        input.ButtonY      = (hold >> 18) & 0x1;
        input.ButtonLeft   = (hold >> 19) & 0x1;
        input.ButtonRight  = (hold >> 20) & 0x1;
        input.ButtonUp     = (hold >> 21) & 0x1;
        input.ButtonDown   = (hold >> 22) & 0x1;
        input.ButtonZL     = (hold >> 23) & 0x1;
        input.ButtonZR     = (hold >> 24) & 0x1;
        input.ButtonL      = (hold >> 25) & 0x1;
        input.ButtonR      = (hold >> 26) & 0x1;
        input.ButtonPlus   = (hold >> 27) & 0x1;
        input.ButtonMinus  = (hold >> 28) & 0x1;
        input.ButtonHome   = (hold >> 29) & 0x1;
        input.ButtonSync   = (hold >> 30) & 0x1;
    }
} // namespace LibMacchiato
