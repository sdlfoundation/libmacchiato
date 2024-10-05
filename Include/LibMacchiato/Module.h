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

#include "CallbackData.h"
#include "Input.h"
#include "Log.h"
#include "Patch.h"

#include "Event.h"

#include <cstring>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace LibMacchiato {
    class IDropdownEntry {
      public:
        virtual void processInput(VPADInput input) = 0;

        virtual ~IDropdownEntry() = default;
    };

    struct ChoiceDropdownEntry : public IDropdownEntry {
      private:
        ChoiceDropdownEntry(std::vector<std::string> choices,
                            std::string&             selectedChoice)
            : choices(choices)
            , selectedChoice(selectedChoice) {}

      public:
        [[nodiscard]] static std::shared_ptr<ChoiceDropdownEntry>
        create(std::string& selectedChoice, std::vector<std::string> choices) {
            return std::make_shared<ChoiceDropdownEntry>(
                ChoiceDropdownEntry(choices, selectedChoice));
        }

        void processInput(VPADInput input) override final {
            auto it = std::ranges::find_if(
                this->choices, [this](const std::string& str) {
                    return str == this->selectedChoice;
                });

            if (it == this->choices.end()) {
                MFATAL("selectedChoice has invalid index");
                return;
            }

            size_t selectedChoiceIdx = std::distance(this->choices.begin(), it);

            if (input.ButtonA) {
                selectedChoiceIdx++;

                if (selectedChoiceIdx >= this->choices.size())
                    selectedChoiceIdx = 0;
            }

            this->selectedChoice = this->choices[selectedChoiceIdx];
        }

        std::vector<std::string> choices;
        std::string&             selectedChoice;
        bool                     focused;
    };

    struct CallbackDropdownEntry : public IDropdownEntry {
      private:
        explicit CallbackDropdownEntry(std::function<void()> callback)
            : callback(callback) {}

      public:
        [[nodiscard]] static std::shared_ptr<CallbackDropdownEntry>
        create(std::function<void()> callback) {
            return std::make_shared<CallbackDropdownEntry>(
                CallbackDropdownEntry(callback));
        }

        void processInput(VPADInput input) override final {
            if (input.ButtonRight) {
                this->callback();
            }
        }

        std::function<void()> callback;
    };

    struct ToggleDropdownEntry : public IDropdownEntry {
      private:
        ToggleDropdownEntry(bool& toggled)
            : toggled(toggled) {}

      public:
        [[nodiscard]] static std::shared_ptr<ToggleDropdownEntry>
        create(bool& toggled) {
            return std::make_shared<ToggleDropdownEntry>(
                ToggleDropdownEntry(toggled));
        }

        void processInput(VPADInput input) override final {
            if (input.ButtonA) {
                this->toggled = !this->toggled;
            }
        }

        bool& toggled;
    };

    struct DropdownComponent {
        DropdownComponent(std::string                     label,
                          std::shared_ptr<IDropdownEntry> entry)
            : label(std::move(label))
            , entry(std::move(entry)) {}

        std::string                     label;
        std::shared_ptr<IDropdownEntry> entry;
    };

    struct Dropdown {
        std::vector<DropdownComponent> components = {};

        template <typename EntryType, typename... Args>
        void insert(std::string label, Args&&... args) {
            this->components.emplace_back(
                std::move(label),
                EntryType::create(std::forward<Args>(args)...));
        }

        void processInput(VPADInput input) {
            for (const auto& component : this->components) {
                component.entry->processInput(input);
            }
        }
    };

    class IDependencyState {
      public:
        virtual void onEnable() {}

        virtual void onUpdate(CallbackData data) {}
        virtual void onDraw(CallbackData data) {}

        virtual void onDisable() {}

        virtual std::optional<Dropdown> dropdown() { return std::nullopt; }

        virtual ~IDependencyState() = default;
    };

    struct Dependency {
      private:
        explicit Dependency(
            std::optional<std::shared_ptr<IDependencyState>> state,
            bool enabled, std::vector<std::string_view> dependencies,
            Patch patch, std::vector<TrampolinePatch> events, bool isCheat)
            : enabled(enabled)
            , state(state)
            , dependencies(dependencies)
            , patch(patch)
            , events(events)
            , isCheat(isCheat) {}

        bool enabled;

      public:
        [[nodiscard]] static Dependency create() {
            return Dependency(std::nullopt, false, {}, Patch::create(), {},
                              false);
        }

        template <typename State, typename... Args>
        // Builders
        [[nodiscard]] inline Dependency&&
        withState(Args&&... args) && noexcept {
            auto state  = State{std::forward<Args>(args)...};
            this->state = std::make_shared<State>(state);
            return std::move(*this);
        }

        template <typename Event, typename... Args>
        // Builders
        [[nodiscard]] inline Dependency&&
        withEvent(Args&&... args) && noexcept {
            auto event   = Event{std::forward<Args>(args)...};
            this->events = event.subscriptions();
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

        [[nodiscard]] inline Dependency&& withIsCheat() && noexcept {
            this->isCheat = true;
            return std::move(*this);
        }

        std::optional<std::shared_ptr<IDependencyState>> state;

        std::vector<std::string_view> dependencies;

        Patch patch;

        // std::vector<std::shared_ptr<LibMacchiato::IEvent>> events;
        std::vector<LibMacchiato::TrampolinePatch> events;

        bool isCheat;

        [[nodiscard]] inline bool isEnabled() const noexcept {
            return this->enabled;
        }

        inline void enable() {
            if (this->enabled)
                return;

            this->enabled = true;
            this->patch.enable();

            for (auto& event : this->events) {
                event.enable();
            }

            if (this->state.has_value())
                this->state.value()->onEnable();

            MDBGINFO("Enabled module \"", this->name, "\"");
        }

        inline void disable() {
            if (!this->enabled)
                return;

            this->enabled = false;
            this->patch.disable();

            for (auto& event : this->events) {
                event.disable();
            }

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
        explicit Module(std::string_view                name,
                        std::optional<std::string_view> description,
                        bool autoEnabled, bool forceEnabled, Dependency dep)
            : name(name)
            , description(description)
            , autoEnabled(autoEnabled)
            , forceEnabled(forceEnabled)
            , dep(std::move(dep)) {}

        std::string_view                name;
        std::optional<std::string_view> description;

        bool autoEnabled;
        bool forceEnabled;

      public:
        Dependency dep;

        [[nodiscard]] static Module create(std::string_view name,
                                           Dependency       dep) {
            return Module(name, std::nullopt, false, false, dep);
        }

        [[nodiscard]] inline Module&&
        withDescription(std::string_view description) && noexcept {
            this->description = description;
            return std::move(*this);
        }

        [[nodiscard]] inline Module&& withAutoEnabled() && noexcept {
            this->autoEnabled = true;
            return std::move(*this);
        }

        [[nodiscard]] inline bool isAutoEnabled() const noexcept {
            return this->autoEnabled;
        }

        [[nodiscard]] inline Module&& withForceEnabled() && noexcept {
            this->forceEnabled = true;
            return std::move(*this);
        }

        [[nodiscard]] inline const std::string_view& getName() const noexcept {
            return this->name;
        }

        inline void enable() { this->dep.enable(); }

        inline void disable() {
            if (this->forceEnabled)
                return;

            this->dep.disable();
        }

        inline void toggle() {
            if (!this->forceEnabled)
                this->dep.toggle();
        }
    };
} // namespace LibMacchiato

#define MODULE(name) [[nodiscard]] inline ::LibMacchiato::Module name()
#define DEPENDENCY(name) [[nodiscard]] ::LibMacchiato::Dependency name()
#define STATE(name)                                                            \
    [[nodiscard]] struct name : public ::LibMacchiato::IDependencyState

#define MODULE_DEF(name) [[nodiscard]] ::LibMacchiato::Module name();
