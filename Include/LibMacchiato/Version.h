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

#include <cassert>
#include <charconv>
#include <expected>
#include <format>
#include <optional>
#include <sdl-utils/Types.h>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>

namespace LibMacchiato {
    inline std::optional<u16> stringToU16(const std::string& str) {
        u16 value;
        auto [ptr, ec] =
            std::from_chars(str.data(), str.data() + str.size(), value);

        if (ec == std::errc() && ptr == str.data() + str.size())
            return value;
        else
            return std::nullopt;
    }

    struct VersionTriplet {
        u16 major = 1;
        u16 minor = 0;
        u16 patch = 0;

        inline std::string getVersionString() {
            return std::format("{}.{}.{}", this->major, this->minor,
                               this->patch);
        }

        u16& operator[](size_t index) {
            switch (index) {
            case 0:
                return this->major;
            case 1:
                return this->minor;
            case 2:
                return this->patch;
            default:
                break;
            }

            throw std::out_of_range("Index out of range");
        }

        bool operator==(const VersionTriplet& rhs) {
            return this->major == rhs.major && this->minor == rhs.minor
                   && this->patch == rhs.patch;
        }

        bool operator!=(const VersionTriplet& rhs) { return !(*this == rhs); }

        static std::expected<VersionTriplet, std::string>
        fromStr(std::string s) {
            if (std::string_view(s).starts_with("v")) {
                s.erase(0, 1);
            }

            std::vector<std::string> tokens;
            std::stringstream        ss(s);
            std::string              item;

            while (std::getline(ss, item, '.')) {
                tokens.push_back(item);
            }

            VersionTriplet triplet;

            size_t i = 0;
            for (const auto& token : tokens) {
                std::optional<u16> maybeToken = stringToU16(token);
                if (!maybeToken.has_value()) {
                    return std::unexpected<std::string>(token);
                }

                triplet[i] = maybeToken.value();
                i++;
            }

            return triplet;
        }
    };
} // namespace LibMacchiato
