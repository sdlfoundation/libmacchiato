#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

namespace LibMacchiato::Utils {
    inline std::string getCurrentDate() {
        auto        now   = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);

        std::tm local_tm = *std::localtime(&now_c);
        char    buffer[11];
        std::strftime(buffer, sizeof(buffer), "%b %d %Y", &local_tm);

        return std::string(buffer);
    }

    inline std::string getCurrentTime() {
        auto        now   = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);

        std::tm local_tm = *std::localtime(&now_c);
        char    buffer[9];
        std::strftime(buffer, sizeof(buffer), "%H:%M:%S", &local_tm);

        return std::string(buffer);
    }
} // namespace LibMacchiato::Utils
