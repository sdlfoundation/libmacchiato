#pragma once

#include "../Kernel/Defs.h"

#include <sdl-utils/Types.h>
#include <stdint.h>

namespace LibMacchiato::Utils::Kernel {
    void copyData(u32 dst, u32 src, u32 len);

    void readPageTableEntry(u32 outputAddr, s32 length);
    void writePageTableEntry(u32 inputAddr, s32 length);

    void patchSysCall(u32 index, u32 addr);
} // namespace LibMacchiato::Utils::Kernel
