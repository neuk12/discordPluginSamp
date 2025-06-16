#include "../lib/samp-plugin-sdk/amx/amx.h"
#include "../lib/samp-plugin-sdk/plugincommon.h"
#include "../lib/samp-plugin-sdk/amxplugin.cpp"

#include <windows.h>
#include <cstdio>
#include "DiscordNatives.hpp"
#include <algorithm>




typedef void (*logprintf_t)(char* format, ...);
logprintf_t logprintf;


extern void *pAMXFunctions;

void PrintColor(const char* text, WORD color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    WORD savedAttrs = csbi.wAttributes;
    SetConsoleTextAttribute(hConsole, (savedAttrs & 0xF0) | color);
    printf("%s", text);
    SetConsoleTextAttribute(hConsole, savedAttrs);
}


AMX_NATIVE_INFO PluginNatives[] = {
    // discord 
    { "DiscordSendMessage", DiscordSendMessage },
    { 0, 0 }
};




PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
    return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}


PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
    pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
    logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

    PrintColor("\n", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    PrintColor("=====================================================================\n", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    PrintColor("                         Discord Plugin Loaded                         \n", FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    PrintColor("=====================================================================\n", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    PrintColor("   Status  : ", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    PrintColor("SUCCESS\n", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    PrintColor("   Author  : ", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    PrintColor("[n3uk]\n", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    PrintColor("   Version : ", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    PrintColor("1.0.0\n", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    PrintColor("=====================================================================\n\n", FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    return true;
}


PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
    PrintColor("[Discord Plugin] ", FOREGROUND_RED | FOREGROUND_INTENSITY);
    PrintColor("Plugin unloaded.\n", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

}


PLUGIN_EXPORT int PLUGIN_CALL AmxLoad( AMX *amx )
{
    return amx_Register(amx, PluginNatives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload( AMX *amx )
{
    return AMX_ERR_NONE;
}
