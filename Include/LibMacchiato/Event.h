#pragma once

namespace Macchiato {
    struct EventSubscription {
        void* subscriber;
        u32   function;
    };

    class IEvent {
      public:
        // Bind methods to function trampoline callbacks
        virtual std::vector<EventSubscription> subscriptions() = 0;

        virtual ~Event() = default;
    };
} // namespace Macchiato
