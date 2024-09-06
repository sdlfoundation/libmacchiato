#pragma once

#include <array>
#include <chrono>
#include <coreinit/debug.h>
#include <coreinit/memdefaultheap.h>
#include <ctime>
#include <deque>
#include <filesystem>
#include <format>
#include <iomanip>
#include <iostream>
#include <list>
#include <nn/erreula.h>
#include <optional>
#include <sstream>
#include <string>
#include <sys/unistd.h>

namespace LibMacchiato::Log {
    constexpr size_t MAX_LOG_SIZE = 100;

    struct LoggerData {
        std::string macchiatoLogPath = "fs:/vol/external01/macchiato/logs";
        std::optional<std::string> logFileName = std::nullopt;
        std::deque<std::string>    recentLogs  = {};

        inline void addRecentLog(std::string log) {
            if (this->recentLogs.empty()) {
                this->recentLogs.resize(MAX_LOG_SIZE, "");
            }

            this->recentLogs.pop_front();
            this->recentLogs.push_back(log);
        }
    };

    extern LoggerData loggerData;

// Implementation macros
// Only meant to be used internally by Macchiato
// =============================================================================
#define SRCLOC (std::string(__FILE__) + ":" + std::to_string(__LINE__)).c_str()

#define MLOGINFO_IMPL(severity, show, ...)                                     \
    ::LibMacchiato::Log::LogInfo {                                             \
        std::chrono::high_resolution_clock::now(), __FILE__, __LINE__,         \
            severity, ::std::format(__VA_ARGS__), show, u""                    \
    }

#define MLOGINFO(severity, show, ...) MLOGINFO_IMPL(severity, show, __VA_ARGS__)

#define MLOGIMPL(severity, show, ...)                                          \
    do {                                                                       \
        ::LibMacchiato::Log::log(MLOGINFO(severity, show, __VA_ARGS__));       \
    } while (0)

#define MSHOWLOG(severity, ...) MLOGIMPL(severity, true, __VA_ARGS__)

#define MLOG(severity, ...) MLOGIMPL(severity, false, __VA_ARGS__)
// =============================================================================

// Macros that print without logging to the screen
#define MINFO(...) MLOG(::LibMacchiato::Log::LogSeverity::Info, __VA_ARGS__)
#define MWARN(...) MLOG(::LibMacchiato::Log::LogSeverity::Warning, __VA_ARGS__)
#define MERROR(...) MLOG(::LibMacchiato::Log::LogSeverity::Error, __VA_ARGS__)
#define MFATAL(...) MLOG(::LibMacchiato::Log::LogSeverity::Crash, __VA_ARGS__)

// Macros that print with logging to the screen
#define MSHOWINFO(...)                                                         \
    MSHOWLOG(::LibMacchiato::Log::LogSeverity::Info, __VA_ARGS__)
#define MSHOWWARN(...)                                                         \
    MSHOWLOG(::LibMacchiato::Log::LogSeverity::Warning, __VA_ARGS__)
#define MSHOWERROR(...)                                                        \
    MSHOWLOG(::LibMacchiato::Log::LogSeverity::Error, __VA_ARGS__)

// Debug macros
#ifdef NDEBUG
#define MDBGLOG(...)
#define MDBGINFO(...)
#define MDBGWARN(...)
#define MDBGERROR(...)
#define MDBGFATAL(...)
#else
#define MDBGLOG(severity, ...) MLOGIMPL(severity, true, __VA_ARGS__)
#define MDBGINFO(...)                                                          \
    MDBGLOG(::LibMacchiato::Log::LogSeverity::Info, __VA_ARGS__)
#define MDBGWARN(...)                                                          \
    MDBGLOG(::LibMacchiato::Log::LogSeverity::Warning, __VA_ARGS__)
#define MDBGERROR(...)                                                         \
    MDBGLOG(::LibMacchiato::Log::LogSeverity::Error, __VA_ARGS__)
#define MDBGFATAL(...)                                                         \
    MDBGLOG(::LibMacchiato::Log::LogSeverity::Crash, __VA_ARGS__)
#endif

    enum class LogSeverity {
        Crash,
        Error,
        Warning,
        Info,
        Verbose
    };

    inline std::string logSeverityToStr(LogSeverity severity) {
        switch (severity) {
        case LogSeverity::Crash:
            return "Crash";
        case LogSeverity::Error:
            return "Error";
        case LogSeverity::Warning:
            return "Warning";
        case LogSeverity::Info:
            return "Info";
        case LogSeverity::Verbose:
            return "Verbose";
        }

        return "Invalid";
    }

    struct LogInfo {
        std::chrono::high_resolution_clock::time_point logTime;
        const char*                                    sourceFile;
        unsigned                                       lineNum;
        LogSeverity                                    severity;
        std::string                                    message;
        bool                                           show;
        const char16_t*                                u16message;
    };

    void log(LogInfo logMsg);

    /*
    inline void init() {
        static FSClient fsClient = {};
        if (FSAddClient(&fsClient, FS_ERROR_FLAG_NONE) != FS_STATUS_OK) {
            MCRASH("Failed to add FSClient");
        }

        // Create erreula
        nn::erreula::CreateArg createArg;
        createArg.region   = nn::erreula::RegionType::Europe;
        createArg.language = nn::erreula::LangType::English;
        createArg.workMemory =
            MEMAllocFromDefaultHeap(nn::erreula::GetWorkMemorySize());
        createArg.fsClient = &fsClient;

        if (!nn::erreula::Create(createArg)) {
            MCRASH("Unable to create erreula instance");
        }
    }
    */
} // namespace LibMacchiato::Log
