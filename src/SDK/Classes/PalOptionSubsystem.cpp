#include "SDK/Classes/PalOptionSubsystem.h"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UFunction.hpp"
#include "Unreal/UScriptStruct.hpp"
#include "Unreal/Property/FStructProperty.hpp"

using namespace RC;
using namespace RC::Unreal;

namespace Palworld {
    void UPalOptionSubsystem::ApplyWorldSettings()
    {
        static auto Function = UObjectGlobals::StaticFindObject<UFunction*>(nullptr, nullptr, STR("/Script/Pal.PalOptionSubsystem:ApplyWorldSettings"));

        if (!Function) {
            throw std::runtime_error{ "Function /Script/Pal.PalOptionSubsystem:ApplyWorldSettings not found." };
        }

        this->ProcessEvent(Function, nullptr);
    }

    void UPalOptionSubsystem::SetOptionWorldSettings(void* SettingsDataPtr)
    {
        static auto Function = UObjectGlobals::StaticFindObject<UFunction*>(nullptr, nullptr, STR("/Script/Pal.PalOptionSubsystem:SetOptionWorldSettings"));

        if (!Function) {
            throw std::runtime_error{ "Function /Script/Pal.PalOptionSubsystem:SetOptionWorldSettings not found." };
        }

        void* ParamsBuffer = FMemory::Malloc(Function->GetParmsSize());
        FMemory::Memzero(ParamsBuffer, Function->GetParmsSize());

        for (auto Property : Function->ForEachPropertyInChain())
        {
            void* ValuePtr = Property->ContainerPtrToValuePtr<void>(ParamsBuffer);

            if (Property->GetName() == STR("InOptionWorldSettings"))
            {
                if (auto StructProperty = CastField<FStructProperty>(Property))
                {
                    auto Size = StructProperty->GetStruct()->GetStructureSize();
                    FMemory::Memcpy(ValuePtr, SettingsDataPtr, Size);
                }
            }
        }

        this->ProcessEvent(Function, ParamsBuffer);
    }
}