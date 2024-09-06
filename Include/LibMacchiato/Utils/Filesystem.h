#pragma once

#include <wut_types.h>

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <sdl-utils/Types.h>

namespace LibMacchiato::Utils::FS {
    constexpr std::string_view MACCHIATO_BASE_PATH =
        "fs:/vol/external01/macchiato";

    bool fileExists(std::string_view filePath);

    bool createDirectory(std::string_view path);

    std::optional<FILE*>           openFile(std::string_view path);
    std::optional<std::vector<u8>> readFile(std::string_view path);
    bool writeFile(std::string_view path, std::string_view contents);

    std::optional<std::string> getSerialId();
}; // namespace LibMacchiato::Utils::FS
