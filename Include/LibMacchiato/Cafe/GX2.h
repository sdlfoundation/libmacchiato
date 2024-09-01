#pragma once

#include "Define.h"

CAFE_FUNCTION(GX2GetCurrentScanBuffer, void, GX2ScanTarget scanTarget,
              GX2ColorBuffer* cb);
CAFE_FUNCTION(GX2MarkScanBufferCopied, void, GX2ScanTarget scanTarget);

namespace LibMacchiato::Cafe {
    inline void initGX2() {
        CREATE_HANDLE(gx2);

        LOAD_CAFE_FUNC(gx2, GX2GetCurrentScanBuffer);
        LOAD_CAFE_FUNC(gx2, GX2MarkScanBufferCopied);
    }
} // namespace LibMacchiato::Cafe
