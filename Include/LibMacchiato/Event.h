#pragma once

#include "Patch/Trampoline.h"

namespace LibMacchiato {
    struct EventSubscription {
        void* subscriber;
        u32   function;
    };

    class IEvent {
      public:
        // Bind methods to function trampoline callbacks
        virtual std::vector<TrampolinePatch> subscriptions() = 0;

        virtual ~IEvent() = default;
    };
} // namespace LibMacchiato

#define UPDATE_EVENT_TRAMPOLINE(func)                                          \
    STATIC_TRAMPOLINE(func, void, void* param_1, void* param_2)

#define INSTALL_UPDATE_EVENT_TRAMPOLINE(func)                                  \
    INSTALL_TRAMPOLINE(0x0e8b05c0, func)

#define UPDATE_EVENT_RETURN(func) return orig_##func(param_1, param_2)
