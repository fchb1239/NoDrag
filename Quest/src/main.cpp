#include "main.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include <stdlib.h>
#include <string>

static ModInfo modInfo;

#define INFO(value...) getLogger().info(value)
#define ERROR(value...) getLogger().error(value)


Logger &getLogger()
{
    static Logger *logger = new Logger(modInfo);
    return *logger;
}

std::string toLower(std::string string) {
    for (int i = 0; i < string.length(); i++)
      {
          string[i] = tolower(string[i]);
      }
    return string;
}

bool modWork = false;

MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(FlightController_FixedUpdate, "", "FlightController", "FixedUpdate", void, Il2CppObject* self)
{
    FlightController_FixedUpdate(self);
    if(modWork){
        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "dragFactor", 0.0f));
    }else{
        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "dragFactor", 10.0f));
    }
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
    
    INSTALL_HOOK(getLogger(), FlightController_FixedUpdate);
    INSTALL_HOOK(getLogger(), GameManager_OnPrivateOKButtonPressed);

    logger.info("Installed all hooks!");
}
