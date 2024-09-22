#pragma once

#include "Kernel.h"

#include <coreinit/cache.h>
#include <coreinit/memorymap.h>

#include <sdl-utils/Types.h>

#include <cstdlib>
#include <cstring>
#include <functional>
#include <string.h>

namespace LibMacchiato::Utils::Memory {
    template <typename Return, typename... Args>
    [[nodiscard]] inline std::function<Return(Args...)>
    copyFunction(uintptr_t src, size_t size) {
        void* dst = std::aligned_alloc(alignof(std::max_align_t), size);
        if (!dst) {
            throw std::bad_alloc();
        }

#ifndef MACCHIATO_TARGET_EMU
        ::LibMacchiato::Utils::Kernel::copyData(
            OSEffectiveToPhysical((u32)dst), OSEffectiveToPhysical(src), size);
#else
        std::memcpy(dst, reinterpret_cast<void*>(src), size);
#endif

        Return (*dstFuncPtr)(Args...) =
            reinterpret_cast<Return (*)(Args...)>(dst);

        std::function<Return(Args...)> dstFunc =
            [dstFuncPtr](Args... args) -> Return {
            return dstFuncPtr(std::forward<Args>(args)...);
        };

        return dstFunc;
    }

    [[nodiscard]] inline void* copyFunction(uintptr_t src, size_t size) {
        void* dst = std::aligned_alloc(alignof(std::max_align_t), size);
        if (!dst) {
            std::abort();
            throw std::bad_alloc();
        }

#ifndef MACCHIATO_TARGET_EMU
        ::LibMacchiato::Utils::Kernel::copyData(
            OSEffectiveToPhysical((u32)dst), OSEffectiveToPhysical(src), size);
#else
        std::memcpy(dst, reinterpret_cast<void*>(src), size);
#endif

        return dst;
    }

    inline void invalidateICache(uintptr_t address, size_t bytes) {
#ifndef MACCHIATO_TARGET_EMU
        ICInvalidateRange((void*)address, bytes);
#else
#endif
    }

    inline void invalidateDCache(uintptr_t address, size_t bytes) {
#ifndef MACCHIATO_TARGET_EMU
        DCInvalidateRange((void*)address, bytes);
#else
#endif
    }

    inline void writeInstruction(u32 address, u32 bytes) {
#ifndef MACCHIATO_TARGET_EMU
        ::LibMacchiato::Utils::Kernel::copyData(
            OSEffectiveToPhysical(address), OSEffectiveToPhysical((u32)&bytes),
            sizeof(u32));
        invalidateICache(address, sizeof(u32));
#endif
    }

    inline u32 readU32(u32 address) {
        u32 bytes = 0;

#ifndef MACCHIATO_TARGET_EMU
        ::LibMacchiato::Utils::Kernel::copyData(
            OSEffectiveToPhysical((u32)&bytes), OSEffectiveToPhysical(address),
            sizeof(u32));
#endif

        return bytes;
    }

    template <typename Data> inline Data read(auto address) {
        Data data;

#ifndef MACCHIATO_TARGET_EMU
        ::LibMacchiato::Utils::Kernel::copyData(
            OSEffectiveToPhysical((u32)&data),
            OSEffectiveToPhysical(reinterpret_cast<u32>(address)),
            sizeof(Data));
#endif

        return data;
    }


    inline std::pair<u16, u16> splitAddress(uintptr_t addr) {
        auto upper = static_cast<u16>((addr >> 16) & 0xFFFF);
        auto lower = static_cast<u16>(addr & 0xFFFF);

        return {upper, lower};
    }
} // namespace LibMacchiato::Utils::Memory
