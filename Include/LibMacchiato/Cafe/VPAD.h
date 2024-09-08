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

CAFE_FUNCTION(VPADRead, s32, VPADChan chan, VPADStatus* buffers, u32 count,
              VPADReadError* error);

namespace LibMacchiato::Cafe {
    inline void initVPAD() {
        CREATE_HANDLE(vpad);

        LOAD_CAFE_FUNC(vpad, VPADRead);
    }
} // namespace LibMacchiato::Cafe
