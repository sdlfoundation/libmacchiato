#pragma once

namespace LibMacchiato {
    [[nodiscard]] inline uintptr_t address(uintptr_t addr) noexcept {
#ifdef MACCHIATO_TARGET_EMU
        if (addr >= 0x10503000 && addr <= 0x106EC713)
            addr -= 0x503000;

        else if (addr >= 0x0e000000 && addr <= 0xEDD0670)
            addr -= 0xC000000;
#endif

        return addr;
    }
} // namespace LibMacchiato