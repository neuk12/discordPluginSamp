#include "discord_logs.hpp"
#include <cstring>  
#include <string>


cell n_discord(AMX* amx, cell* params)
{

    char* canal = nullptr;
    char* mensaje = nullptr;
    amx_StrParam(amx, params[1], canal);
    amx_StrParam(amx, params[2], mensaje);

    if (canal == nullptr || mensaje == nullptr)
        return 0;

    SendDiscordLog(canal, mensaje);

    return 1;
}
