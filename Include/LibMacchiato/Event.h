#pragma once

#include "Patch/Trampoline.h"

namespace LibMacchiato {
    struct EventSubscription {
        void* subscriber;
        u32   function;
    };

    class IEvent {
      public:
        virtual std::vector<TrampolinePatch> subscriptions() = 0;

        virtual ~IEvent() = default;
    };
} // namespace LibMacchiato
