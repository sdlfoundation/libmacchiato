#include "LibMacchiato/Utils/Kernel.h"

#include <coreinit/cache.h>
#include <coreinit/memorymap.h>

extern "C" void SCKernelCopyData(u32 dst, u32 src, u32 len);
extern "C" void KernelWriteSRsInternal(sr_table_t* table);
extern "C" void KernelReadSRsInternal(sr_table_t* table);

namespace LibMacchiato::Utils::Kernel {
    namespace {
        constexpr uintptr_t KERN_SYSCALL_TBL1 = 0xFFE84C70; // Unknown
        constexpr uintptr_t KERN_SYSCALL_TBL2 = 0xFFE85070; // Games
        constexpr uintptr_t KERN_SYSCALL_TBL3 = 0xFFE85470; // Loader
        constexpr uintptr_t KERN_SYSCALL_TBL4 = 0xFFEAAA60; // Home menu
        constexpr uintptr_t KERN_SYSCALL_TBL5 = 0xFFEAAE60; // Browser

        extern "C" void __attribute__((noinline))
        copyDataInternal(u32 dst, u32 src, u32 len) {
            asm volatile("li %r0, 0x2500\n"
                         "sc\n"
                         "blr\n");
        }

        void __attribute__((noinline)) kernWrite(void* addr, u32 value) {
            asm volatile("stwu 1, -0x10(1)\n"
                         "stw 2,  0x08(1)\n"
                         "stw 13, 0x0C(1)\n"
                         "li 3,1\n"
                         "li 4,0\n"
                         "mr 5,%1\n"
                         "li 6,0\n"
                         "li 7,0\n"
                         "lis 8,1\n"
                         "mr 9,%0\n"
                         "mr %1,1\n"
                         "li 0,0x3500\n"
                         "sc\n"
                         "nop\n"
                         "mr 1,%1\n"
                         "lwz 2, 0x8(1)\n"
                         "lwz 13, 0xC(1)\n"
                         "addi 1, 1, 0x10\n"
                         :
                         : "r"(addr), "r"(value)
                         : "memory", "ctr", "lr", "0", "3", "4", "5", "6", "7",
                           "8", "9", "10", "11", "12");
        }
    } // namespace

    void copyData(u32 dst, u32 src, u32 len) {
        copyDataInternal(dst, src, len);
    }

    void readPageTableEntry(u32 outputAddr, s32 len) {
        auto dst = (u32)OSEffectiveToPhysical(outputAddr);
        u32  src = 0xFFE20000;
        ICInvalidateRange(&dst, 4);
        DCFlushRange(&dst, 4);
        DCFlushRange(&src, 4);
        copyData(dst, src, len);
        DCFlushRange((void*)outputAddr, len);
        ICInvalidateRange((void*)outputAddr, len);
    }

    void writePageTableEntry(u32 inputAddr, s32 len) {
        u32  dst = 0xFFE20000;
        auto src = (u32)OSEffectiveToPhysical(inputAddr);
        ICInvalidateRange(&src, 4);
        DCFlushRange(&src, 4);
        copyData(dst, src, len);
    }

    void patchSysCall(int index, u32 addr) {
        kernWrite((void*)(KERN_SYSCALL_TBL1 + index * 4), addr);
        kernWrite((void*)(KERN_SYSCALL_TBL2 + index * 4), addr);
        kernWrite((void*)(KERN_SYSCALL_TBL3 + index * 4), addr);
        kernWrite((void*)(KERN_SYSCALL_TBL4 + index * 4), addr);
        kernWrite((void*)(KERN_SYSCALL_TBL5 + index * 4), addr);
    }

    // void KernelNOPAtPhysicalAddress(u32 addr) {
    //     u32 dst = 0x60000000;
    //     ICInvalidateRange(&dst, 4);
    //     DCFlushRange(&dst, 4);
    //     copyData(addr, (u32)OSEffectiveToPhysical((u32)&dst),
    //                    4);
    // }
} // namespace LibMacchiato::Utils::Kernel
