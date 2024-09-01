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
    typedef std::function<void()>                 DirectoryButton;
    typedef std::variant<Module, DirectoryButton> DirectoryEntryComponent;

    struct DirectoryEntry {
      private:
        explicit DirectoryEntry(std::string_view        label,
                                DirectoryEntryComponent component)
            : label(label)
            , component(component) {}

      public:
        static DirectoryEntry create(std::string_view        label,
                                     DirectoryEntryComponent component) {
            return DirectoryEntry(label, component);
        }

        std::string_view        label;
        DirectoryEntryComponent component;
    };

    struct Directory {
      private:
        explicit Directory(std::string_view            name,
                           std::vector<DirectoryEntry> entries)
            : name(name)
            , entries(entries) {}

      public:
        [[nodiscard]] static Directory create(std::string_view name) {
            return Directory(name, {});
        }

        // Add a module.
        inline void mod(std::function<LibMacchiato::Module()> mod) {
            LibMacchiato::Module finalMod = mod();

            this->entries.push_back(
                DirectoryEntry::create(finalMod.getName(), finalMod));
        }

        // Add a button.
        inline void btn(std::string                      label,
                        std::function<DirectoryButton()> btn) {
            this->entries.push_back(DirectoryEntry::create(label, btn()));
        }

        std::string_view            name;
        std::vector<DirectoryEntry> entries;
    };

    struct ExportDirectoryRoot {
      private:
        explicit ExportDirectoryRoot(
            std::vector<Directory>                directories,
            std::vector<LibMacchiato::Dependency> dependencies)
            : directories(directories)
            , dependencies(dependencies) {}

      public:
        std::vector<Directory>                directories;
        std::vector<LibMacchiato::Dependency> dependencies;

        [[nodiscard]] static ExportDirectoryRoot create() {
            return ExportDirectoryRoot({}, {});
        }

        inline void dir(std::function<Directory()> dir) {
            this->directories.push_back(dir());
        }

        inline void dep(std::function<LibMacchiato::Dependency()> dep) {
            this->dependencies.push_back(dep());
        }
    };
} // namespace LibMacchiato

#define DIRECTORY(name) [[nodiscard]] ::LibMacchiato::Directory name()

#define EXPORT_ROOT(name)                                                      \
    [[nodiscard]] ::LibMacchiato::ExportDirectoryRoot name()
