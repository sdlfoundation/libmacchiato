#pragma once

#include "../Assert.h"
#include "../Log.h"

#include "../Utils/Assembly.h"
#include "../Utils/Bind.h"
#include "../Utils/Kernel.h"

#include "Hook.h"
#include "Line.h"

#include <array>
#include <concepts>
#include <cstdlib>
#include <cstring>
#include <expected>
#include <functional>
#include <optional>
#include <sdl-utils/Types.h>

#include <coreinit/cache.h>
#include <coreinit/memorymap.h>

namespace LibMacchiato {
    /*
     * Trampoline patches / hooks divert the code execution to a custom
     * function provided by the user ("repl function"). The hook saves the
     * original starting bytes of the function which are overwritten in order to
     * jump to the custom function. The bytes are save in a program-allocated
     * function pointer `orig_XXXX()`. This function pointer can be called
     * inside the custom function to access the original function's
     * functionality.
     *
     * In other words, trampolines do the following:
     *
     * original_function:
     *     b hook_function  ; Branch to hook function
     *     nop              ; No operation (placeholder for original
     *     instructions)
     *
     * hook_function:
     *     ; Custom code here
     *     ; ...
     *
     *     b trampoline_function  ; Branch to trampoline function
     *
     * trampoline_function:
     *     ; Execute saved original instructions
     *     mflr r0
     *     stw r0, 4(r1)
     *     stwu r1, -16(r1)
     *     ; ...
     *
     *     b original_function + 8  ; Branch back to the original function after
     *     the overwritten instructions
     */
    struct TrampolinePatch {
      private:
        TrampolinePatch(uintptr_t origFunctionAddress, void* origFunction,
                        const void* replFunction, Hook hook)
            : origFunctionAddress(origFunctionAddress)
            , origFunction(origFunction)
            , replFunction(replFunction)
            , hook(hook) {}

        uintptr_t   origFunctionAddress;
        void*       origFunction;
        const void* replFunction;
        Hook        hook;

      public:
        inline void enable() { this->hook.enable(); }
        inline void disable() { this->hook.disable(); }

        template <typename Return, typename... Args>
        [[nodiscard]] static TrampolinePatch
        create(uintptr_t   address, Return (*&origFunction)(Args...),
               const void* replFunction) {
            // If the function starts with a jump, that probably means that the
            // function has already been hooked. In that case, recursively
            // update the address to point to the previous trampoline.
            // address =
            //     Utils::Assembly::getAdjustedAddressIfFirstInstructionIsBranch(
            //         address);

            const Hook hook = Hook::create(address, replFunction);

            std::vector<u32> trampBytes = {};

            for (const auto& patch : hook.getBranchData()) {
                const u32  assembly       = patch.getDisableAssembly();
                const u32  opcode         = assembly & 0xFC000003;
                const bool opcodeIsBranch = opcode == 0x48000000;

                if (!opcodeIsBranch) {
                    trampBytes.push_back(assembly);
                    continue;
                }

                u32 jumpOffset = assembly & 0x01FFFFFC;

                // Sign extension check
                if ((jumpOffset & 0x02000000) != jumpOffset) {
                    jumpOffset += 0xFE000000;
                }

                const u32 absoluteJumpAddress =
                    jumpOffset + static_cast<u32>(patch.getAddress());

                const std::expected<u32, PPCAssembler::AssembleError>
                    shortBranch = PPCAssembler::assemble(
                        std::format("ba {}", absoluteJumpAddress));

                if (shortBranch.has_value()) {
                    trampBytes.push_back(shortBranch.value());
                    continue;
                }

                std::vector<u32> jumpToOrigBytes =
                    Utils::Assembly::jump(absoluteJumpAddress);

                trampBytes.insert(trampBytes.end(), jumpToOrigBytes.begin(),
                                  jumpToOrigBytes.end());
            }

            const u32 remainingFunctionStart =
                address + hook.getBranchData().size() * sizeof(u32);
            const std::vector<u32> jumpBackBytes =
                Utils::Assembly::jump(remainingFunctionStart);

            trampBytes.insert(trampBytes.end(), jumpBackBytes.begin(),
                              jumpBackBytes.end());

            //
            // MINFO("size:", std::to_string(hook.getBranchData().size()));
            // size_t i = 0;
            // for (const auto byte : trampBytes) {
            //     MINFO("#", i, ": ", std::to_string(byte));
            //     i++;
            // }
            //

            const size_t trampBytesSize = trampBytes.size() * sizeof(u32);

            void* mem = reinterpret_cast<void*>(new u8[trampBytesSize]);
            // void* mem = MEMAllocFromExpHeapEx(gJumpHeapHandle,
            // trampBytesSize,
            //                                   sizeof(u32));
            if (!mem) {
                MFATAL("Failed to allocate memory for trampoline patch.");
            }

            Utils::Kernel::copyData(
                OSEffectiveToPhysical(reinterpret_cast<u32>(mem)),
                OSEffectiveToPhysical(reinterpret_cast<u32>(trampBytes.data())),
                trampBytesSize);

            Utils::Memory::invalidateICache(reinterpret_cast<u32>(mem),
                                            trampBytesSize);

            origFunction =
                reinterpret_cast<Return (*)(Args...)>(std::move(mem));

            return TrampolinePatch(address,
                                   reinterpret_cast<void*>(origFunction),
                                   replFunction, std::move(hook));
        }
    };

#define TRAMPOLINE(name, res, ...)                                             \
    res (*orig_##name)(__VA_ARGS__) __attribute__((section(".data")));         \
    res repl_##name(__VA_ARGS__)

#define INSTALL_TRAMPOLINE(address, name)                                      \
    ::Macchiato::TrampolinePatch::create(address, orig_##name,                 \
                                         reinterpret_cast<void*>(repl_##name))

#define INSTALL_FUNC_TRAMPOLINE(func, name)                                    \
    ::Macchiato::TrampolinePatch::create(reinterpret_cast<uintptr_t>(func),    \
                                         orig_##name,                          \
                                         reinterpret_cast<void*>(repl_##name))

#define INSTALL_CAFE_TRAMPOLINE(func, name)                                    \
    ::Macchiato::TrampolinePatch::create(                                      \
        reinterpret_cast<uintptr_t>(dyn_##func), orig_##name,                  \
        reinterpret_cast<void*>(repl_##name))
} // namespace LibMacchiato
