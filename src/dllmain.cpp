#include <Mod/CppUserModBase.hpp>
#include <UE4SSProgram.hpp>
#include <cstdint>
#include <memory>
#include <safetyhook.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UEnum.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/GameplayStatics.hpp>
#include <Unreal/Property/FBoolProperty.hpp>
#include <Unreal/Property/FEnumProperty.hpp>
#include <Unreal/Property/NumericPropertyTypes.hpp>

#include <Signatures.hpp>
#include <SigScanner/SinglePassSigScanner.hpp>

// Palworld SDK
#include <SDK/Classes/PalMapObjectDamageReactionComponent.h>
#include <SDK/Classes/PalMapObjectUtility.h>
#include <SDK/Classes/PalUtility.h>
#include <SDK/Classes/PalOptionSubsystem.h>

#include <ModConfig.h>

using namespace RC;
using namespace RC::Unreal;
using namespace Palworld;

std::vector<SignatureContainer> SigContainer;
SinglePassScanner::SignatureContainerMap SigContainerMap;

bool EnablePvP = false;

typedef bool(__thiscall* TYPE_PalUtility_IsPvP)(UObject*);
static inline TYPE_PalUtility_IsPvP PalUtility_IsPvP_Internal;

// Signature stuff, expect them to break with updates
void BeginScan()
{
    SignatureContainer PalUtility_IsPvP_Signature = [=]() -> SignatureContainer {
        return {
            {{ "48 83 EC 28 E8 ?? ?? ?? ?? 0F B6 80 B5 00 00 00 48 83 C4 28 C3"}},
            [=](SignatureContainer& self) {
                void* FunctionPointer = static_cast<void*>(self.get_match_address());

                PalUtility_IsPvP_Internal =
                    reinterpret_cast<TYPE_PalUtility_IsPvP>(FunctionPointer);

                self.get_did_succeed() = true;

                return true;
            },
            [](const SignatureContainer& self) {
                if (!self.get_did_succeed())
                {
                    Output::send<LogLevel::Error>(STR("Failed to find signature for UPalUtility::IsPvP\n"));
                }
            }
        };
    }();

    SigContainer.emplace_back(PalUtility_IsPvP_Signature);
    SigContainerMap.emplace(ScanTarget::MainExe, SigContainer);
    SinglePassScanner::start_scan(SigContainerMap);
}

SafetyHookInline PalUtility_IsPvP_Hook{};
bool __stdcall PalUtility_IsPvP(UObject* WorldContextObject)
{
    return EnablePvP;
}

class YetAnotherPvPFix : public RC::CppUserModBase
{
public:
    YetAnotherPvPFix() : CppUserModBase()
    {
        ModName = STR("YetAnotherPvPFix");
        ModVersion = STR("1.4.0");
        ModDescription = STR("Fixes PvP.");
        ModAuthors = STR("Okaetsu");

        Output::send<LogLevel::Verbose>(STR("{} v{} by {} loaded.\n"), ModName, ModVersion, ModAuthors);

        BeginScan();

        PalUtility_IsPvP_Hook = safetyhook::create_inline(reinterpret_cast<void*>(PalUtility_IsPvP_Internal),
            reinterpret_cast<void*>(PalUtility_IsPvP));

        auto& Settings = PVP::Config::Settings::get();
        Settings.deserialize();

        EnablePvP = Settings.PVP.EnablePvP;

        if (EnablePvP)
        {
            Output::send<LogLevel::Verbose>(STR("[YAPP] Enabling PvP.\n"));
        }
        else
        {
            Output::send<LogLevel::Verbose>(STR("[YAPP] Disabling PvP.\n"));
        }
    }

    ~YetAnotherPvPFix() override
    {
    }

    auto on_update() -> void override
    {
    }

    auto on_unreal_init() -> void override
    {
        Output::send<LogLevel::Verbose>(STR("[{}] loaded successfully!\n"), ModName);

        auto& Settings = PVP::Config::Settings::get();

        static bool HasInitialized = false;
        Hook::RegisterBeginPlayPostCallback([&](AActor* Actor) {
            if (HasInitialized) return;
            HasInitialized = true;

            auto OptionSubsystem = UPalUtility::GetOptionSubsystem(Actor);
            auto Property = OptionSubsystem->GetPropertyByNameInChain(STR("OptionWorldSettings"));
            if (!Property)
            {
                Output::send<LogLevel::Error>(STR("[{}] failed to load, property 'OptionWorldSettings' doesn't exist in OptionSubsystem anymore.\n"), ModName);
                return;
            }

            auto OptionWorldSettingsProperty = CastField<FStructProperty>(Property);
            if (!OptionWorldSettingsProperty)
            {
                Output::send<LogLevel::Error>(STR("[{}] failed to load, property 'OptionWorldSettings' is not a Struct Property?\n"), ModName);
                return;
            }

            auto OptionWorldSettingsStruct = OptionWorldSettingsProperty->GetStruct();
            if (!OptionWorldSettingsStruct)
            {
                Output::send<LogLevel::Error>(STR("[{}] failed to load, property 'OptionWorldSettings' is not a valid Struct?\n"), ModName);
                return;
            }

            auto OptionWorldSettings = OptionWorldSettingsProperty->ContainerPtrToValuePtr<void>(OptionSubsystem);
            if (!OptionWorldSettings)
            {
                Output::send<LogLevel::Error>(STR("[{}] failed to load, could not get internal data for property 'OptionWorldSettings'.\n"), ModName);
                return;
            }

            if (Settings.PVP.EnablePvP)
            {
                auto bEnablePlayerToPlayerDamage_Property = OptionWorldSettingsStruct->GetPropertyByName(STR("bEnablePlayerToPlayerDamage"));
                if (!bEnablePlayerToPlayerDamage_Property)
                {
                    Output::send<LogLevel::Error>(STR("[{}] failed to load, property 'bEnablePlayerToPlayerDamage' doesn't exist in OptionWorldSettings anymore.\n"), ModName);
                    return;
                }

                auto bEnablePlayerToPlayerDamage = CastField<FBoolProperty>(bEnablePlayerToPlayerDamage_Property);
                if (!bEnablePlayerToPlayerDamage)
                {
                    Output::send<LogLevel::Error>(STR("[{}] failed to load, property 'bEnablePlayerToPlayerDamage' is not a bool anymore.\n"), ModName);
                    return;
                }

                bEnablePlayerToPlayerDamage->SetPropertyValueInContainer(OptionWorldSettings, true);
                OptionSubsystem->SetOptionWorldSettings(OptionWorldSettings);
                OptionSubsystem->ApplyWorldSettings();
                Output::send<LogLevel::Verbose>(STR("[YAPP] Enabling PvP Damage to Players.\n"));
            }
            else
            {
                Output::send<LogLevel::Verbose>(STR("[YAPP] Disabling PvP Damage to Players.\n"));
            }
        });
    }
};


#define YETANOTHERPVPFIX_API __declspec(dllexport)
extern "C"
{
    YETANOTHERPVPFIX_API RC::CppUserModBase* start_mod()
    {
        return new YetAnotherPvPFix();
    }

    YETANOTHERPVPFIX_API void uninstall_mod(RC::CppUserModBase* mod)
    {
        delete mod;
    }
}
