#pragma once

#include "LibMacchiato/Input.h"
#include "LibMacchiato/Log.h"
#include "LibMacchiato/Module.h"
#include "LibMacchiato/Patch.h"

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <typeindex>
#include <variant>
#include <vector>

namespace LibMacchiato {
    constexpr size_t DEFAULT_MODULE_LIMIT    = 100;
    constexpr size_t DEFAULT_DIRECTORY_LIMIT = 20;

    typedef std::function<void()>                 DirectoryButton;
    typedef std::variant<Module, DirectoryButton> DirectoryEntryComponent;

    struct ExportDirectoryEntry {
      private:
        explicit ExportDirectoryEntry(std::string_view        label,
                                      DirectoryEntryComponent component)
            : label(label)
            , component(component) {}

      public:
        static ExportDirectoryEntry create(std::string_view        label,
                                           DirectoryEntryComponent component) {
            return ExportDirectoryEntry(label, component);
        }

        std::string_view        label;
        DirectoryEntryComponent component;
    };

    struct ExportDirectory {
      private:
        explicit ExportDirectory(std::string_view                  name,
                                 std::vector<ExportDirectoryEntry> entries)
            : name(name)
            , entries(entries) {}

      public:
        [[nodiscard]] static ExportDirectory create(std::string_view name) {
            return ExportDirectory(name, {});
        }

        // Add a module.
        inline void mod(std::function<LibMacchiato::Module()> mod) {
            LibMacchiato::Module finalMod = mod();

            this->entries.push_back(
                ExportDirectoryEntry::create(finalMod.getName(), finalMod));
        }

        // Add a button.
        inline void btn(std::string                      label,
                        std::function<DirectoryButton()> btn) {
            this->entries.push_back(ExportDirectoryEntry::create(label, btn()));
        }

        std::string_view                  name;
        std::vector<ExportDirectoryEntry> entries;
    };

    struct ExportDirectoryRoot {
      private:
        std::vector<ExportDirectory>          directories;
        std::vector<LibMacchiato::Dependency> dependencies;

        explicit ExportDirectoryRoot(
            std::vector<ExportDirectory>          directories,
            std::vector<LibMacchiato::Dependency> dependencies)
            : directories(directories)
            , dependencies(dependencies) {}

      public:
        [[nodiscard]] static ExportDirectoryRoot create() {
            return ExportDirectoryRoot({}, {});
        }

        inline void dir(std::function<ExportDirectory()> dir) {
            this->directories.push_back(dir());
        }

        inline void dep(std::function<LibMacchiato::Dependency()> dep) {
            this->dependencies.push_back(dep());
        }
    };

#define DIRECTORY(name) [[nodiscard]] ::LibMacchiato::ExportDirectory name()
#define ROOT(name) [[nodiscard]] ::LibMacchiato::ExportDirectoryRoot name()
} // namespace LibMacchiato
