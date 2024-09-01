#pragma once

#include "Cafe/GX2.h"
#include "Cafe/VPAD.h"

namespace LibMacchiato::Cafe {
    inline void initLibraries() {
        initGX2();
        initVPAD();
    }
} // namespace LibMacchiato::Cafe
