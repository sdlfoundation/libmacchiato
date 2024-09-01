#pragma once

#include "../Assembler/Mask.h"
#include "../Utils/Assembly.h"
#include "../Utils/Memory.h"
#include "Hook.h"
#include "Line.h"

#include <coreinit/cache.h>
#include <coreinit/memorymap.h>

#include <sdl-utils/Types.h>

#include <algorithm>
#include <expected>
#include <vector>

namespace LibMacchiato {
    struct AssemblyHook {
      private:
        explicit AssemblyHook(Hook hook, void* hookFunction)
            : hook(hook)
            , hookFunction(hookFunction) {}

        Hook  hook;
        void* hookFunction;

      public:
        inline void enable() { this->hook.enable(); }
        inline void disable() { this->hook.disable(); }

        [[nodiscard]] static AssemblyHook create(uintptr_t        address,
                                                 std::vector<u32> assemblies,
                                                 bool keepOriginalBytes) {
            std::vector<u32> bytes = assemblies;

            void* mem = reinterpret_cast<void*>(new u32[bytes.size() + 32]);

            if (!mem)
                MFATAL("Failed to allocate memory for trampoline patch.");

            Hook hook = Hook::create(address, mem);

            size_t i = 0;

            if (keepOriginalBytes) {
                for (const auto& patch : hook.getBranchData()) {
                    u32 assembly = patch.getDisableAssembly();
                    bytes.push_back(assembly);
                }
            }

            std::vector<u32> jumpToOrigBytes = Utils::Assembly::jump(
                address + hook.getBranchData().size() * sizeof(u32));

            bytes.insert(bytes.end(), jumpToOrigBytes.begin(),
                         jumpToOrigBytes.end());

            Utils::Kernel::copyData(
                OSEffectiveToPhysical(reinterpret_cast<u32>(mem)),
                OSEffectiveToPhysical(reinterpret_cast<u32>(bytes.data())),
                bytes.size());

            Utils::Memory::invalidateICache(reinterpret_cast<u32>(mem),
                                            bytes.size());

            return AssemblyHook(hook, mem);
        }

        [[nodiscard]] static AssemblyHook
        assemble(uintptr_t address, std::vector<std::string> instructions,
                 bool keepOriginalBytes) {
            std::vector<u32> assemblies = {};

            for (const auto& instruction : instructions) {
                assemblies.push_back(
                    PPCAssembler::assemble(instruction).value());
            }

            return AssemblyHook::create(address, assemblies, keepOriginalBytes);
        }
    };
} // namespace LibMacchiato
