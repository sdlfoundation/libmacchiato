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

#include "LibMacchiato/Log.h"
#include "LibMacchiato/Utils/Filesystem.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>
#include <whb/log.h>

namespace LibMacchiato::Log {
    LoggerData loggerData;

    void generateLogFileName() {
        const auto p1 = std::chrono::system_clock::now();

        auto time = std::chrono::duration_cast<std::chrono::seconds>(
                        p1.time_since_epoch())
                        .count();

#ifndef NDEBUG
        std::string filePrefix = "dbg-";
#else
        std::string filePrefix = "rel-";
#endif

        std::string fileName = filePrefix + std::to_string(time) + ".txt";

        loggerData.logFileName = fileName;
    }

    void log(LogInfo logMsg) {
        if (strlen(logMsg.sourceFile) == 0)
            std::abort();

        std::time_t time = std::chrono::system_clock::to_time_t(logMsg.logTime);
        std::tm*    localtime = std::localtime(&time);
        std::stringstream output;

        output << "[" << logSeverityToStr(logMsg.severity) << "] "
               << "{" << std::put_time(localtime, "%H:%M:%S") << "} "
               << logMsg.message;

#ifndef NDEBUG
        output << " /" << logMsg.sourceFile << ":"
               << std::to_string(logMsg.lineNum) << ")";
#endif

        output << std::endl;

        std::string outputStr = output.str();

        // WHBLogPrintf(outputStr.c_str());
        OSReport(outputStr.c_str());

        if (logMsg.severity == LogSeverity::Crash) {
            usleep(100);
            std::abort();
        }
    }
} // namespace LibMacchiato::Log
