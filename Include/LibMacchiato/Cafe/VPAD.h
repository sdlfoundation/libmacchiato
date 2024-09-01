#pragma once

#include "Define.h"

CAFE_FUNCTION(VPADRead, s32, VPADChan chan, VPADStatus* buffers, u32 count,
              VPADReadError* error);

namespace LibMacchiato::Cafe {
    inline void initVPAD() {
        CREATE_HANDLE(vpad);

        LOAD_CAFE_FUNC(vpad, VPADRead);
    }
} // namespace LibMacchiato::Cafe
