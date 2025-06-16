#ifndef DISCORD_HPP
#define DISCORD_HPP

#include "../lib/samp-plugin-sdk/amx/amx.h"

cell n_discord(AMX* amx, cell* params);
void SendDiscordLog(const char* canal, const char* message);

#endif // DISCORD_HPP
