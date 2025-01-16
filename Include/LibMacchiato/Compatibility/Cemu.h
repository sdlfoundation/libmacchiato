#pragma once

namespace LibMacchiato::Compatibility {
    inline void updateAddressIfCemu(uintptr_t& address) {
#ifdef MACCHIATO_TARGET_EMU
        if (address >= 0x0e000000 && address <= 0x0edd0670) {
            address -= 0xc000000;
        }
#endif
    }
} // namespace LibMacchiato::Compatibility
