#include "main.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

static ModInfo modInfo;

#define INFO(value...) getLogger().info(value)
#define ERROR(value...) getLogger().error(value)


Logger &getLogger()
{
    static Logger *logger = new Logger(modInfo);
    return *logger;
}

MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(FlightController_Start, "", "FlightController", "Start", void, Il2CppObject* self)
{
    FlightController_Start(self);
    CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "dragFactor", 0.0f));
}

extern "C" void setup(ModInfo &info)
{
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
}

extern "C" void load()
{
    il2cpp_functions::Init();
    Logger& logger = getLogger();
    logger.info("Installing hooks...");
    
    INSTALL_HOOK(getLogger(), FlightController_Start);

    logger.info("Installed all hooks!");
}