#pragma once

#include <cstdint>

namespace LibMacchiato::Utils {
    template <typename... Args>
    inline bool isInBounds(uintptr_t innerBounds, uintptr_t outerBounds,
                           Args... args) {
        return ((reinterpret_cast<uintptr_t>(&args) >= innerBounds
                 && reinterpret_cast<uintptr_t>(&args) <= outerBounds)
                && ...);
    }

    template <typename... Args>
    inline bool isAboveBounds(uintptr_t bounds, Args... args) {
        return ((reinterpret_cast<uintptr_t>(args) >= bounds) && ...);
    }

    template <typename... Args>
    inline bool isBelowBounds(uintptr_t bounds, Args... args) {
        return ((reinterpret_cast<uintptr_t>(args) < bounds) && ...);
    }
} // namespace LibMacchiato::Utils
