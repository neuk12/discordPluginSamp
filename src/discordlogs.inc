#if defined _discord_included
    #endinput
#endif
#define _discord_included

/**
 * Envía un mensaje a un webhook de Discord configurado en el archivo discord.ini
 *
 * @param canal   El nombre del canal (clave en el archivo INI) donde se enviará el mensaje.
 * @param mensaje El contenido del mensaje a enviar.
 *
 * @return 1 si se envió correctamente, 0 si falló.
 */
native DiscordSendMessage(const canal[], const mensaje[]);
