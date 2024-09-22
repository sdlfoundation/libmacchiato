#pragma once

#include "Patch/Trampoline.h"

namespace Macchiato {
    struct EventSubscription {
        void* subscriber;
        u32   function;
    };

    class IEvent {
      public:
        // Bind methods to function trampoline callbacks
        virtual std::vector<TrampolinePatch> subscriptions() = 0;

        virtual ~Event() = default;
    };
} // namespace Macchiato
