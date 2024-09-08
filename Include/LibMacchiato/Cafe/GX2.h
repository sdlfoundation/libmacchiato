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
