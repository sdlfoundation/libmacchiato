/*
 * Code is a modified and extended version of Wii U Environment's filesystem
 * utilities.
 *
 * https://github.com/wiiu-env
 */

#include "LibMacchiato/Utils/Filesystem.h"
#include "LibMacchiato/Log.h"

#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <format>
#include <fstream>
#include <malloc.h>
#include <optional>
#include <string_view>
#include <sys/fcntl.h>
#include <sys/unistd.h>
#include <vector>

#include <coreinit/debug.h>
#include <coreinit/mcp.h>

namespace LibMacchiato::Utils::FS {
    bool fileExists(const std::string_view fullpath) {
        struct stat filestat {};

        char dirnoslash[strlen(fullpath.data()) + 2];
        snprintf(dirnoslash, sizeof(dirnoslash), "%s", fullpath.data());

        while (dirnoslash[strlen(dirnoslash) - 1] == '/') {
            dirnoslash[strlen(dirnoslash) - 1] = '\0';
        }

        char* notRoot = strrchr(dirnoslash, '/');
        if (!notRoot) {
            strcat(dirnoslash, "/");
        }

        if (stat(dirnoslash, &filestat) == 0) {
            return true;
        }

        return false;
    }

    bool createDirectory(const std::string_view fullpath) {
        int32_t result = 0;

        char dirnoslash[fullpath.length() + 1];
        strcpy(dirnoslash, fullpath.data());

        auto pos = strlen(dirnoslash) - 1;
        while (dirnoslash[pos] == '/') {
            dirnoslash[pos] = '\0';
            pos--;
        }

        if (fileExists(dirnoslash)) {
            return true;
        } else {
            char parentpath[strlen(dirnoslash) + 2];
            strcpy(parentpath, dirnoslash);
            char* ptr = strrchr(parentpath, '/');

            if (!ptr) {
                // Device root directory (must be with '/')
                strcat(parentpath, "/");
                struct stat filestat {};
                if (stat(parentpath, &filestat) == 0) {
                    return true;
                }

                return false;
            }

            ptr++;
            ptr[0] = '\0';

            result = createDirectory(parentpath);
        }

        if (!result)
            return false;

        if (mkdir(dirnoslash, 0777) < 0) {
            return false;
        }

        return true;
    }

    // TODO: Better error handling without exceptions.
    // bool copyFile(const std::string_view in, const std::string_view out) {
    //     std::ifstream src(in.data(), std::ios::binary);
    //     std::ofstream dst(out.data(), std::ios::binary);
    //
    //     dst << src.rdbuf();
    //
    //     // ...
    //
    //     return true;
    // }

    std::optional<std::string> getSerialId() {
        alignas(0x40) MCPSysProdSettings settings{};
        auto                             handle = MCP_Open();
        if (handle >= 0) {
            if (MCP_GetSysProdSettings(handle, &settings) == 0) {
                std::string serialID =
                    std::string(settings.code_id) + settings.serial_id;
                return serialID;
            } else {
                MERROR("Failed to get serial ID");
            }
            MCP_Close(handle);
        }

        MERROR("MCP_Open failed");

        return std::nullopt;
    }

    std::optional<FILE*> openFile(std::string_view path) {
        FILE* file = fopen(path.data(), "w");
        if (!file)
            return std::nullopt;
        return file;
    }

    std::optional<std::vector<u8>> readFile(FILE* file, size_t size) {
        std::vector<u8> buffer = {};
        buffer.resize(size);

        size_t read = fread(buffer.data(), size, size, file);

        if (read != buffer.size()) {
            fclose(file);
            return std::nullopt;
        }

        if (fclose(file) != 0) {
            return std::nullopt;
        }

        return buffer;
    }

    bool writeFile(FILE* file, std::string_view contents) {
        size_t written = fwrite(contents.data(), 1, contents.size(), file);
        if (written != contents.size()) {
            fclose(file);
            return false;
        }

        if (fclose(file) != 0) {
            return false;
        }

        return true;
    }

    /*
     * @brief Writes a log to the target file in any directory.
     */
    bool writeFile(std::string_view path, std::string_view contents) {
        FILE* file = fopen(path.data(), "w");
        if (!file) {
            return false;
        }

        size_t written = fwrite(contents.data(), 1, contents.size(), file);
        if (written != contents.size()) {
            fclose(file);
            return false;
        }

        if (fclose(file) != 0) {
            return false;
        }

        return true;
    }
} // namespace LibMacchiato::Utils::FS
