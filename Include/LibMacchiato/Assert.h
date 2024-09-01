#pragma once

#include "Log.h"

namespace Macchiato {
#define MASSERT(cond, out)                                                     \
    do {                                                                       \
        if (!(cond)) {                                                         \
            MFATAL(out);                                                       \
        }                                                                      \
    } while (0)

#ifndef NDEBUG
#define MDBGASSERT(cond, out)                                                  \
    if (!cond) {                                                               \
        MFATAL(out);                                                           \
    }
#else
#define MDBGASSERT(cond, out)
#endif
} // namespace Macchiato
