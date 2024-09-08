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

#include <math.h>
#include <sdl-utils/Types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <iterator>
#include <optional>
#include <string>
#include <vector>

#include <sdl-utils/Types.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace LibMacchiato::ELF {
    struct Elf32_Ehdr {
        u8  e_ident[0x10];
        u16 e_type;
        u16 e_machine;
        u32 e_version;
        u32 e_entry;
        u32 e_phoff;
        u32 e_shoff;
        u32 e_flags;
        u16 e_ehsize;
        u16 e_phentsize;
        u16 e_phnum;
        u16 e_shentsize;
        u16 e_shnum;
        u16 e_shstrndx;
    };

    struct Elf32_Shdr {
        u32 sh_name;
        u32 sh_type;
        u32 sh_flags;
        u32 sh_addr;
        u32 sh_offset;
        u32 sh_size;
        u32 sh_link;
        u32 sh_info;
        u32 sh_addralign;
        u32 sh_entsize;
    };

    struct Elf32_Sym {
        u32 st_name;
        u32 st_value;
        u32 st_size;
        u8  st_info;
        u8  st_other;
        u16 st_shndx;
    };

    struct Elf32_Rela {
        u32 r_offset;
        u32 r_info;
        s32 r_addend;
    };

    enum Elf32_PhdrType : uint32_t {
        PT_NULL    = 0x00000000, // Unused segment
        PT_LOAD    = 0x00000001, // Loadable segment
        PT_DYNAMIC = 0x00000002, // Dynamic linking information
        PT_INTERP  = 0x00000003, // Interpreter information
        PT_NOTE    = 0x00000004, // Auxiliary information
        PT_SHLIB   = 0x00000005, // Reserved
        PT_PHDR    = 0x00000006, // Program header table itself
        PT_TLS     = 0x00000007, // Thread-Local Storage template
        PT_NUM     = 0x00000008, // Number of defined types
        PT_LOOS    = 0x60000000, // OS-specific range
        PT_GNU_EH_FRAME =
            0x6474e550, // GNU-specific: Exception handling information
        PT_GNU_STACK =
            0x6474e551, // GNU-specific: Indicates stack executability
        PT_GNU_RELRO = 0x6474e552, // GNU-specific: Read-only after relocation
        PT_LOSUNW    = 0x6ffffffa, // Sun-specific low bound
        PT_SUNWBSS   = 0x6ffffffa, // Sun-specific: .bss section
        PT_SUNWSTACK = 0x6ffffffb, // Sun-specific: Stack segment
        PT_HISUNW    = 0x6fffffff, // Sun-specific high bound
        PT_HIOS      = 0x6fffffff, // OS-specific high bound
        PT_LOPROC    = 0x70000000, // Processor-specific low bound
        PT_HIPROC    = 0x7fffffff  // Processor-specific high bound
    };

    struct Elf32_Phdr {
        Elf32_PhdrType p_type;   // Segment type
        u32            p_offset; // Offset of the segment in the file
        u32 p_vaddr;  // Virtual address where the segment should be loaded in
                      // memory
        u32 p_paddr;  // Physical address (unused in most systems, for systems
                      // without virtual memory)
        u32 p_filesz; // Number of bytes in the file image of the segment
        u32 p_memsz;  // Number of bytes in the memory image of the segment
        u32 p_flags;  // Segment flags (e.g., executable, writable)
        u32 p_align;  // Alignment of the segment in memory
    };

    std::optional<u32>
    findExportedFunctionVirtualAddress(const void* loadedRplData,
                                       const char* functionName);

    std::optional<u32>
    getFileOffsetFromVirtualAddress(const Elf32_Ehdr* ehdr,
                                    uintptr_t         virtualAddress);
} // namespace LibMacchiato::ELF
