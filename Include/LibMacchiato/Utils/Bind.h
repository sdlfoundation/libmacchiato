#pragma once

#include "../Log.h"
#include "OS.h"

#include <string>

#define CREATE_HANDLE(lib)                                                     \
    OSDynLoad_Module lib;                                                      \
    {                                                                          \
        std::string libWithPrefix = std::string(#lib) + ".rpl";                \
        OSDynLoad_Acquire(libWithPrefix.data(), &lib);                         \
    }

#define LOAD_FUNC(handle, func)                                                \
    {                                                                          \
        OSDynLoad_Error err =                                                  \
            OSDynLoad_FindExport(handle, OS_DYNLOAD_EXPORT_FUNC, #func,        \
                                 reinterpret_cast<void**>(&func));             \
        if (err != OSDynLoad_Error::OS_DYNLOAD_OK) {                           \
            MFATAL("Couldn't find function \"", #func,                         \
                   "\" with error code: ", std::to_string(err));               \
        }                                                                      \
    }

#define LOAD_CAFE_FUNC(handle, func)                                           \
    {                                                                          \
        OSDynLoad_Error err = OSDynLoad_FindExport(                            \
            handle, OS_DYNLOAD_EXPORT_FUNC, #func, (void**)(&(dyn_##func)));   \
        if (err != OSDynLoad_Error::OS_DYNLOAD_OK) {                           \
            MFATAL("Couldn't find function \"", #func,                         \
                   "\" with error code: ", std::to_string(err));               \
        }                                                                      \
    }

#ifndef MACCHIATO_TARGET_EMU
#define BIND_FUNC(name, addr, res, ...)                                        \
    using name##_DEF      = res (*)(__VA_ARGS__);                              \
    const name##_DEF name = reinterpret_cast<name##_DEF>(addr)
#else
// Cemu uses an incorrect base virtual address for userspace applications of
// `0x02000000`, whereas real hardware uses `0x0e000000`.
#define BIND_FUNC(name, addr, res, ...)                                        \
    using name##_DEF      = res (*)(__VA_ARGS__);                              \
    const name##_DEF name = reinterpret_cast<name##_DEF>(addr - 0xC000000)
#endif

#ifndef MACCHIATO_TARGET_EMU
#define BIND_VAR(name, addr, ...)                                              \
    using name##_TYPE        = __VA_ARGS__;                                    \
    inline name##_TYPE& name = *(name##_TYPE*)addr
#else
#define BIND_VAR(name, addr, ...)                                              \
    using name##_TYPE        = __VA_ARGS__;                                    \
    inline name##_TYPE& name = *(name##_TYPE*)(addr - 0x503000)
#endif
