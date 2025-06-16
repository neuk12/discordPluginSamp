# SA-MP Discord Webhook Plugin

Un plugin para **San Andreas Multiplayer (SA-MP)** que permite enviar mensajes a canales de **Discord** utilizando **webhooks** configurables mediante un archivo `discord.ini`.

## 📦 Características

- Envío de mensajes a múltiples canales de Discord.
- Configuración de webhooks desde archivo `.ini`.
- Comunicación HTTPS mediante **WinHTTP**.
- Native disponible para Pawn:

```pawn
native DiscordSendMessage(const canal[], const mensaje[]);
