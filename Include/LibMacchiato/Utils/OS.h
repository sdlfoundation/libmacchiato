#pragma once

#include <coreinit/dynload.h>

#include <sdl-utils/Types.h>

namespace LibMacchiato::Utils {
    inline std::optional<OSDynLoad_NotifyData>
    getRplByName(std::string_view name) {
        s32 rplsNum = OSDynLoad_GetNumberOfRPLs();
        if (rplsNum == 0) {
            return std::nullopt;
        }

        std::vector<OSDynLoad_NotifyData> rpls;
        rpls.resize(rplsNum);

        if (!OSDynLoad_GetRPLInfo(0, rplsNum, rpls.data())) {
            return std::nullopt;
        }

        for (const auto& rpl : rpls) {
            if (!rpl.name) {
                continue;
            }

            std::string_view rplName(rpl.name);

            if (rplName.ends_with(name)) {
                return rpl;
            }
        }

        return std::nullopt;
    }

    inline std::optional<u32> getRplDataAddrByName(std::string_view name) {
        auto rpl = getRplByName(name);

        if (rpl.has_value())
            return rpl.value().dataAddr;

        return std::nullopt;
    }

    inline std::optional<u32> getRplTextAddrByName(std::string_view name) {
        auto rpl = getRplByName(name);

        if (rpl.has_value())
            return rpl.value().textAddr;

        return std::nullopt;
    }
} // namespace LibMacchiato::Utils
