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
