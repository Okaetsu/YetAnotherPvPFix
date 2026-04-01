#pragma once

#include <cstdint>
#include <filesystem>

#include <Common.hpp>
#include <File/File.hpp>
#include <IniParser/Ini.hpp>

namespace PVP::Config {
    class Settings
    {
    public:
        struct SectionPVP
        {
            bool EnablePvP;
        } PVP;
    public:
        static auto get() -> Settings&
        {
            static Settings instance;
            return instance;
        }

        auto deserialize() -> void;

        Settings() {};
        Settings(const Settings&);
        Settings& operator=(const Settings&);
    private:
        auto LoadSettings(RC::Ini::Parser&) -> void;
    };
}