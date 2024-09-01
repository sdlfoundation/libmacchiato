#pragma once

#include "../Assembler/Error.h"

namespace LibMacchiato {
    typedef std::variant<PPCAssembler::AssembleError> PatchError;

    [[nodiscard]] inline std::string patchErrorToStr(PatchError patchError) {
        if (auto error = std::get_if<PPCAssembler::AssembleError>(&patchError))
            return PPCAssembler::assembleErrorToStr(*error);

        return "Invalid patch error.";
    }
} // namespace LibMacchiato
