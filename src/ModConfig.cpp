#include <UE4SSProgram.hpp>
#include <Helpers/String.hpp>
#include <ModConfig.h>

using namespace RC;
namespace fs = std::filesystem;

namespace PVP::Config
{
    auto Settings::deserialize() -> void
    {
        auto working_directory = UE4SSProgram::get_program().get_working_directory();
        auto main_directory = fs::path(working_directory) / STR("Mods") / STR("YetAnotherPvPFix");
        auto file_name = main_directory / STR("PVP-settings.ini");
        if (!fs::exists(main_directory))
        {
            fs::create_directories(main_directory);
        }
        
        if (!fs::exists(file_name))
        {
            throw std::runtime_error{ "[YAPP] PVP-settings.ini could not be found" };
        }

        auto file = File::open(file_name, File::OpenFor::Reading, File::OverwriteExistingFile::No, File::CreateIfNonExistent::Yes);
        Ini::Parser parser;
        parser.parse(file);
        file.close();

        LoadSettings(parser);
    }

    auto Settings::LoadSettings(Ini::Parser& parser) -> void
    {
        constexpr static File::CharType section_pvp[] = STR("PVP");
        try
        {
            PVP.EnablePvP = parser.get_bool(section_pvp, L"EnablePvP");
        }
        catch (std::exception& e)
        {
            Output::send<LogLevel::Error>(STR("[YAPP] {}\n"), RC::to_generic_string(e.what()));
        }
    }
}
