#ifndef DISCORD_HPP
#define DISCORD_HPP

#include "../lib/samp-plugin-sdk/amx/amx.h"

void SendDiscordLog(const char* canal, const char* message, const char* colorNombre = "Blanco", const char* mention = "");
cell AMX_NATIVE_CALL n_discord(AMX* amx, cell* params);

#endif // DISCORD_HPP
