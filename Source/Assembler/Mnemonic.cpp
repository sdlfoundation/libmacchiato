#include "LibMacchiato/Assembler/Mnemonic.h"

namespace LibMacchiato::PPCAssembler {
    u32 mnemonicToOpcode(PPCMnemonic mnemonic) {
        switch (mnemonic) {
        case PPCMnemonic::ADDIC:
            return 12;
        case PPCMnemonic::ADDI:
            return 14;
        case PPCMnemonic::ADDIS:
            return 15;
        case PPCMnemonic::LI:
            return 14;
        case PPCMnemonic::LIS:
            return 15;
        case PPCMnemonic::ORI:
            return 24;
        case PPCMnemonic::NOP:
            return 24;
        case PPCMnemonic::B:
            return 18;
        case PPCMnemonic::BA:
            return 18;
        case PPCMnemonic::BL:
            return 18;
        case PPCMnemonic::BLR:
            return 19;
        case PPCMnemonic::BCTR:
            return 19;
        case PPCMnemonic::MTSPR:
            return 31;
        case PPCMnemonic::MTCTR:
            return 31;
        case PPCMnemonic::LWZ:
            return 32;
        case PPCMnemonic::LBZ:
            return 34;
        case PPCMnemonic::STW:
            return 36;
        case PPCMnemonic::STWU:
            return 37;
        case PPCMnemonic::STB:
            return 38;
        default:
            return 0;
        }
    }

    std::optional<PPCMnemonic> strToMnemonic(std::string& s) {
        if (s == "addic")
            return PPCMnemonic::ADDIC;
        if (s == "addi")
            return PPCMnemonic::ADDI;
        if (s == "addis")
            return PPCMnemonic::ADDIS;
        if (s == "li")
            return PPCMnemonic::LI;
        if (s == "lis")
            return PPCMnemonic::LIS;
        if (s == "ori")
            return PPCMnemonic::ORI;
        if (s == "nop")
            return PPCMnemonic::NOP;
        if (s == "b")
            return PPCMnemonic::B;
        if (s == "ba")
            return PPCMnemonic::BA;
        if (s == "bl")
            return PPCMnemonic::BL;
        if (s == "blr")
            return PPCMnemonic::BLR;
        if (s == "bctr")
            return PPCMnemonic::BCTR;
        if (s == "mtspr")
            return PPCMnemonic::MTSPR;
        if (s == "mtctr")
            return PPCMnemonic::MTCTR;
        if (s == "lwz")
            return PPCMnemonic::LWZ;
        if (s == "lwzu")
            return PPCMnemonic::LWZU;
        if (s == "lbz")
            return PPCMnemonic::LBZ;
        if (s == "stw")
            return PPCMnemonic::STW;
        if (s == "stwu")
            return PPCMnemonic::STWU;
        if (s == "stb")
            return PPCMnemonic::STB;

        return std::nullopt;
    };
} // namespace LibMacchiato::PPCAssembler
