#include "LibMacchiato/ELF.h"
#include "LibMacchiato/Log.h"
#include "LibMacchiato/Utils/Filesystem.h"

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
    std::optional<u32>
    findExportedFunctionVirtualAddress(const void* loadedRplData,
                                       const char* functionName) {
        if (!loadedRplData) {
            MERROR("loadedRplData is NULL");
            return std::nullopt;
        }

        if (!functionName) {
            MERROR("functionName is NULL");
            return std::nullopt;
        }

        const Elf32_Ehdr* ehdr =
            reinterpret_cast<const Elf32_Ehdr*>(loadedRplData);

        if (!ehdr) {
            MERROR("ehdr is NULL");
            return std::nullopt;
        }

        const Elf32_Shdr* shdrTable = reinterpret_cast<const Elf32_Shdr*>(
            reinterpret_cast<const uint8_t*>(ehdr) + ehdr->e_shoff);

        if (!shdrTable) {
            MERROR("shdrTable is NULL");
            return std::nullopt;
        }

        const Elf32_Shdr* symtabHdr = nullptr;
        const Elf32_Shdr* strtabHdr = nullptr;

        const Elf32_Shdr& shstrtabHeader = shdrTable[ehdr->e_shstrndx];
        const char*       shstrtab       = reinterpret_cast<const char*>(
            reinterpret_cast<const u8*>(ehdr) + shstrtabHeader.sh_offset);

        for (size_t i = 0; i < ehdr->e_shnum; i++) {
            const Elf32_Shdr& shdr        = shdrTable[i];
            const char*       sectionName = &shstrtab[shdr.sh_name];

            if (strcmp(sectionName, ".symtab") == 0) {
                symtabHdr = &shdr;
            } else if (strcmp(sectionName, ".strtab") == 0) {
                strtabHdr = &shdr;
            }
        }

        if (!symtabHdr) {
            MERROR("symtab is NULL");
            return std::nullopt;
        }

        if (!strtabHdr) {
            MERROR("strtab is NULL");
            return std::nullopt;
        }

        const Elf32_Sym* symtabData = reinterpret_cast<const Elf32_Sym*>(
            reinterpret_cast<const uint8_t*>(ehdr) + symtabHdr->sh_offset);
        const char* strtabData = reinterpret_cast<const char*>(
            reinterpret_cast<const uint8_t*>(ehdr) + strtabHdr->sh_offset);

        if (!symtabData) {
            MERROR("symtabData is NULL");
            return std::nullopt;
        }

        if (!strtabData) {
            MERROR("strtabData is NULL");
            return std::nullopt;
        }

        size_t symCount = symtabHdr->sh_size / sizeof(Elf32_Sym);

        for (size_t i = 0; i < symCount; i++) {
            const Elf32_Sym& sym        = symtabData[i];
            const char*      symbolName = &strtabData[sym.st_name];

            if (strcmp(symbolName, functionName) == 0) {
                return sym.st_value;
            }
        }

        return std::nullopt;
    }

    std::optional<u32>
    getFileOffsetFromVirtualAddress(const Elf32_Ehdr* ehdr,
                                    uintptr_t         virtualAddress) {
        const Elf32_Phdr* programHeaders = reinterpret_cast<const Elf32_Phdr*>(
            reinterpret_cast<const u8*>(ehdr) + ehdr->e_phoff);

        for (size_t i = 0; i < ehdr->e_phnum; i++) {
            const Elf32_Phdr& phdr = programHeaders[i];

            if (phdr.p_type == PT_LOAD) {
                u32 segmentStart = phdr.p_vaddr;
                u32 segmentEnd   = phdr.p_vaddr + phdr.p_memsz;

                // Check if the virtual address is within this segment
                if (virtualAddress >= segmentStart
                    && virtualAddress < segmentEnd) {
                    u32 fileOffset =
                        phdr.p_offset + (virtualAddress - segmentStart);
                    return fileOffset;
                }
            }
        }

        return std::nullopt;
    }
} // namespace LibMacchiato::ELF
