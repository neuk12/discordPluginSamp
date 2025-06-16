
#include "../lib/samp-plugin-sdk/amx/amx.h"
#include "discord_logs.hpp"


cell AMX_NATIVE_CALL DiscordSendMessage(AMX* amx, cell* params)
{
    return n_discord(amx, params);
}


