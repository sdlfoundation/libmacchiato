#pragma once

#include "../Utils/Kernel.h"
#include "../Utils/Memory.h"

#include <optional>

namespace LibMacchiato {
    template <typename T> struct DataPatch {
      private:
        uintptr_t        address;
        T                enableData;
        std::optional<T> disableData;

        explicit DataPatch(uintptr_t address, T enableData,
                           std::optional<T> disableData)
            : address(address)
            , enableData(enableData)
            , disableData(disableData) {}


        inline void apply(bool enable) {
            u32 address = reinterpret_cast<u32>(this->address);

            if (!this->disableData.has_value()) [[unlikely]] {
                this->disableData = T{};

                Utils::Kernel::copyData(
                    OSEffectiveToPhysical((u32)(reinterpret_cast<uintptr_t>(
                        &this->disableData.value()))),
                    address, sizeof(T));

                Utils::Memory::invalidateDCache(
                    reinterpret_cast<uintptr_t>(&this->disableData.value()),
                    sizeof(T));
            }

            T data;

            enable ? data = this->enableData : data = this->disableData.value();

            Utils::Kernel::copyData(OSEffectiveToPhysical(address),
                                    OSEffectiveToPhysical((u32)(&data)),
                                    sizeof(T));
            Utils::Memory::invalidateDCache(address, sizeof(T));
        }

      public:
        inline void enable() { this->apply(true); }
        inline void disable() { this->apply(false); }

        static DataPatch<T> create(uintptr_t address, T data) {
            return DataPatch(address, data, std::nullopt);
        }
    };
} // namespace LibMacchiato
