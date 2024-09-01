#pragma once

#include <sdl-utils/Types.h>

#include <optional>
#include <string>

namespace LibMacchiato::PPCAssembler {
    enum class PPCMnemonic {
        ADDIC,
        ADDI,
        ADDIS,
        B,
        BA,
        BL,
        BLR,
        LI,
        LIS,
        ORI,
        NOP,
        MTSPR,
        MTCTR,
        BCTR,
        STW,
        STWU,
        STB,
        LWZ,
        LWZU,
        LBZ,
    };

    u32                        mnemonicToOpcode(PPCMnemonic mnemonic);
    std::optional<PPCMnemonic> strToMnemonic(std::string& s);
} // namespace LibMacchiato::PPCAssembler
