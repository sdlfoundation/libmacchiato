#pragma once

#include "Address.h"

namespace LibMacchiato::Compatibility {
    inline void updateAddressIfCemu(uintptr_t& addr) {
        addr = LibMacchiato::address(addr);
    }
} // namespace LibMacchiato::Compatibility
