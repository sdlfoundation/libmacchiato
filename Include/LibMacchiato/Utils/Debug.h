#pragma once

#ifndef NDEBUG

namespace LibMacchiato::Utils::Debug {
    inline void infLoop() {
        volatile bool keepRunning = true;
        while (keepRunning) {
        }
    }

#define DBG_BLOCK(code) code

} // namespace LibMacchiato::Utils::Debug

#else

inline void infLoop();
#define DBG_BLOCK(code)

#endif
