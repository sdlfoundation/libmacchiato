#pragma once

#include "ExportResource.h"
#include "Version.h"

#include <functional>
#include <string_view>

namespace LibMacchiato {
    struct DirectoryExportEntry {
        std::string_view name;
    };

    struct MacchiatoExport {
        std::string_view                     pluginName;
        VersionTriplet                       pluginVersion;
        VersionTriplet                       macchiatoApiVersion;
        std::function<ExportDirectoryRoot()> root;
    };
} // namespace LibMacchiato

#define MACCHIATO_EXPORT()                                                     \
    extern "C" ::LibMacchiato::MacchiatoExport MacchiatoExport()
