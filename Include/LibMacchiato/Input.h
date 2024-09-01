#pragma once

#include <expected>
#include <sdl-utils/Types.h>
#include <vpad/input.h>

namespace LibMacchiato {
    struct VPADInput {
        bool unused : 0x1;
        bool StickLEmulationLeft : 0x1;
        bool StickLEmulationRight : 0x1;
        bool StickLEmulationUp : 0x1;
        bool StickLEmulationDown : 0x1;
        bool StickREmulationLeft : 0x1;
        bool StickREmulationRight : 0x1;
        bool StickREmulationUp : 0x1;
        bool StickREmulationDown : 0x1;
        bool unused2 : 0x4;
        bool StickLButton : 0x1;
        bool StickRButton : 0x1;
        bool ButtonTV : 0x1;
        bool ButtonA : 0x1;
        bool ButtonB : 0x1;
        bool ButtonX : 0x1;
        bool ButtonY : 0x1;
        bool ButtonLeft : 0x1;
        bool ButtonRight : 0x1;
        bool ButtonUp : 0x1;
        bool ButtonDown : 0x1;
        bool ButtonZL : 0x1;
        bool ButtonZR : 0x1;
        bool ButtonL : 0x1;
        bool ButtonR : 0x1;
        bool ButtonPlus : 0x1;
        bool ButtonMinus : 0x1;
        bool ButtonHome : 0x1;
        bool ButtonSync : 0x1;

        [[nodiscard]] static std::expected<VPADInput, VPADReadError> read() {
            VPADStatus    vpadStatus = {};
            VPADReadError vpadError;

            VPADRead(VPAD_CHAN_0, &vpadStatus, 1, &vpadError);

            if (vpadError > 0) {
                return std::unexpected<VPADReadError>(vpadError);
            }

            // auto input = static_cast<VPADInput>(vpadStatus.hold);
            VPADInput* input = (VPADInput*)&vpadStatus.hold;

            return *input;
        }
    };

    static_assert(sizeof(VPADInput) == sizeof(u32));
} // namespace LibMacchiato
