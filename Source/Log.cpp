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

        /*
        if (logMsg.show) {
            nn::erreula::AppearArg errorArgs;

            errorArgs.errorArg.errorType    = nn::erreula::ErrorType::Message;
            errorArgs.errorArg.renderTarget = nn::erreula::RenderTarget::Both;
            errorArgs.errorArg.controllerType =
                nn::erreula::ControllerType::DrcGamepad;
            errorArgs.errorArg.button1Label = u"ok";
            switch (logMsg.severity) {
            case LogSeverity::Crash:
                errorArgs.errorArg.errorTitle = u"The application has crashed!";
                break;
            case LogSeverity::Error:
                errorArgs.errorArg.errorTitle = u"An error has occurred!";
                break;
            case LogSeverity::Warning:
                errorArgs.errorArg.errorTitle = u"Warning!";
                break;
            case LogSeverity::Info:
                errorArgs.errorArg.errorTitle = u"Info";
                break;
            case LogSeverity::Verbose:
                errorArgs.errorArg.errorTitle = u"Message";
                break;
            }

            errorArgs.errorArg.errorMessage = logMsg.u16message;
            nn::erreula::AppearErrorViewer(errorArgs);
        }
        */

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
