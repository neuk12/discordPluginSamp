#include "discord_logs.hpp"
#include <cstring>  
#include <string>


cell AMX_NATIVE_CALL n_discord(AMX* amx, cell* params)
{
    char* canal = nullptr;
    char* mensaje = nullptr;
    char* colorStr = nullptr;
    char* mention = nullptr;

    int paramCount = params[0] / sizeof(cell);

    if (paramCount < 2) {
        printf("[DiscordLogs] Error: faltan parámetros obligatorios (canal y mensaje)\n");
        return 0;
    }

    amx_StrParam(amx, params[1], canal);
    amx_StrParam(amx, params[2], mensaje);

    if (!canal || !mensaje) {
        printf("[DiscordLogs] Error: canal o mensaje es nullptr\n");
        return 0;
    }

    if (paramCount >= 3) {
        amx_StrParam(amx, params[3], colorStr);
    } else {
        colorStr = nullptr;
    }

    if (paramCount >= 4) {
        amx_StrParam(amx, params[4], mention);
        if (!mention) mention = (char*)"";
    } else {
        mention = (char*)"";
    }
    SendDiscordLog(canal, mensaje, colorStr, mention);
    return 1;
}
