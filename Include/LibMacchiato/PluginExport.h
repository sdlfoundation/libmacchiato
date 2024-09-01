#pragma once

#include "../Version.h"
#include "Module.h"

#include <functional>
#include <string_view>
#include <vector>

namespace LibMacchiato {
    struct MacchiatoExport {
        std::string_view                         programName;
        VersionTriplet                           programVersion;
        VersionTriplet                           macchiatoApiVersion;
        std::vector<std::function<Module()>>     modules;
        std::vector<std::function<Dependency()>> dependencies;
    };
} // namespace LibMacchiato

#define MACCHIATO_EXPORT() ::LibMacchiato::MacchiatoExport MacchiatoExport()
