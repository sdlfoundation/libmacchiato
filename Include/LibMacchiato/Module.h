#pragma once

#include "Patch.h"

#include <cstring>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace LibMacchiato {
    typedef std::function<void()> ModuleCallback;

    typedef std::variant<std::string, bool, u16, u32, s16, s32>
        DropdownComponent;

    struct IDependencyState {
        virtual void onEnable() {}
        virtual void onUpdate() {}
        virtual void onDisable() {}

        virtual std::vector<DropdownComponent> components() { return {}; }

        virtual ~IDependencyState() = default;
    };

    struct Dependency {
      private:
        explicit Dependency(
            std::optional<std::shared_ptr<IDependencyState>> state,
            bool enabled, std::vector<std::string_view> dependencies,
            Patch patch, bool isCheat)
            : enabled(enabled)
            , state(state)
            , dependencies(dependencies)
            , patch(patch)
            , isCheat(isCheat) {}

        bool enabled = false;

      public:
        /*
         * @brief Create a Macchiato module.
         *
         * @param name The name of the module. If not provided, it will be
         * considered an internal module and always enabled by default during
         * the duration of the program.
         */
        [[nodiscard]] static Dependency create() {
            return Dependency(std::nullopt, false, {}, Patch::create(), false);
        }

        // Builders
        [[nodiscard]] inline Dependency&&
        withState(std::shared_ptr<IDependencyState> state) && noexcept {
            this->state = std::move(state);
            return std::move(*this);
        }

        [[nodiscard]] inline Dependency&& withDependencies(
            std::vector<std::string_view> dependencies) && noexcept {
            this->dependencies = dependencies;
            return std::move(*this);
        }

        [[nodiscard]] inline Dependency&&
        withPatch(Patch nextPatch) && noexcept {
            this->patch.getComponents().insert(
                this->patch.getComponents().end(),
                nextPatch.getComponents().begin(),
                nextPatch.getComponents().end());

            return std::move(*this);
        }

        [[nodiscard]] inline Dependency&&
        withPatch(std::function<Patch()> nextPatch) && noexcept {
            Patch finalPatch = nextPatch();
            this->patch.getComponents().insert(
                this->patch.getComponents().end(),
                finalPatch.getComponents().begin(),
                finalPatch.getComponents().end());
            return std::move(*this);
        }

        [[nodiscard]] inline Dependency&& withCheat() && noexcept {
            this->isCheat = true;
            return std::move(*this);
        }

        std::optional<std::shared_ptr<IDependencyState>> state;

        std::vector<std::string_view> dependencies;

        Patch patch;

        bool isCheat;

        [[nodiscard]] inline bool isEnabled() const noexcept {
            return this->enabled;
        }

        inline void enable() {
            if (this->enabled)
                return;

            this->enabled = true;
            this->patch.enable();

            if (this->state.has_value())
                this->state.value()->onEnable();

            MDBGINFO("Enabled module \"", this->name, "\"");
        }

        inline void disable() {
            if (!this->enabled)
                return;

            this->enabled = false;
            this->patch.disable();

            if (this->state.has_value())
                this->state.value()->onDisable();

            MDBGINFO("Disabled module \"", this->name, "\"");
        }

        inline void toggle() noexcept {
            this->enabled ? this->disable() : this->enable();
        }
    };

    struct Module {
      private:
        explicit Module(Dependency dep, std::string_view name,
                        std::optional<std::string_view> description)
            : dep(std::move(dep))
            , name(name)
            , description(description) {}

        Dependency                      dep;
        std::string_view                name;
        std::optional<std::string_view> description;

        bool autoEnabled;
        bool forceEnabled;

      public:
        [[nodiscard]] static Module create(std::string_view name,
                                           Dependency       dep) {
            return Module(dep, name, std::nullopt);
        }

        [[nodiscard]] inline Module&& withAutoEnabled() && noexcept {
            this->autoEnabled = true;
            return std::move(*this);
        }

        [[nodiscard]] inline Module&& withForceEnabled() && noexcept {
            this->forceEnabled = true;
            return std::move(*this);
        }

        [[nodiscard]] inline const std::string_view& getName() const noexcept {
            return this->name;
        }

        [[nodiscard]] inline const Dependency& getDep() const noexcept {
            return this->dep;
        }

        inline void enable() { this->dep.enable(); }

        inline void disable() {
            if (this->forceEnabled)
                return;

            this->dep.disable();
        }

        inline void toggle() {
            if (!this->forceEnabled && !this->dep.isEnabled())
                this->dep.toggle();
        }
    };
} // namespace LibMacchiato

#define MODULE(name) [[nodiscard]] ::LibMacchiato::Module name()
#define DEPENDENCY(name) [[nodiscard]] ::LibMacchiato::Dependency name()
#define STATE(name)                                                            \
    [[nodiscard]] struct name : public ::LibMacchiato::IDependencyState

#define MODULE_DEF(name) [[nodiscard]] ::LibMacchiato::Module name();
