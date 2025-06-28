# SA-MP Discord Webhook Plugin

Un plugin para **San Andreas Multiplayer (SA-MP)** que permite enviar mensajes a canales de **Discord** utilizando **webhooks** configurables mediante un archivo `discord.ini`.

---

## 📦 Características

- Envío de mensajes a múltiples canales de Discord.  
- Configuración de webhooks desde archivo `.ini`.  
- Comunicación HTTPS mediante **WinHTTP**.  
- Native disponible para Pawn:  
  native DiscordSendMessage(const canal[], const mensaje[]);

---

## 📦 Release: v1.1 - Primer Lanzamiento Estable

### 📑 Descripción

¡Versión estable del **SA-MP Discord Webhook Plugin**!  
Permite a los servidores **SA-MP** enviar mensajes directamente a canales de **Discord** mediante webhooks configurados en un archivo `discord.ini`.

---

### 🆕 Novedades respecto a la versión experimental:

- Versión estable y funcional tras las pruebas iniciales.

- Nueva native con parámetros extendidos:  
  native DiscordSendMessage(const canal[], const mensaje[], const color[] = "Blanco", const mention[] = "");

- Soporta ahora:  
  📍 Color personalizado para el embed (ejemplo: "Verde", "Rojo", "Azul").  
  📍 Menciones directas en Discord (ejemplo: "@here", "@admins").

---

### 📌 Corrección importante

Se solucionó un problema al enviar mensajes con caracteres acentuados o símbolos especiales (como "dinámico") que llegaban a Discord con caracteres corruptos.  

El texto estaba en codificación Windows-1252 (ANSI) y no se convertía correctamente a UTF-8, requerido por Discord.

**Impacto:**  
- Mensajes mostraban caracteres incorrectos.  
- Pérdida de legibilidad y profesionalismo en logs.  
- Podía generar errores o rechazo en la API.

**Solución:**  
- Conversión correcta de Windows-1252 a UTF-8 antes de construir el JSON.  
- Ajuste para que la función de escape JSON y envío usen siempre UTF-8.  
- Validación del entorno para compatibilidad total.

---

### 📄 Ejemplo de uso

DiscordSendMessage("bank", "El jugador depositó $5000.", "Verde");  
DiscordSendMessage("staff", "El admin expulsó a un jugador.", "Rojo", "@here");

---

### 📂 Ejemplo de `discord.ini`

[Discord]  
bank=https://discord.com/api/webhooks/xxxxxxxxxx/xxxxxxxxxxxxxxxx  
staff=https://discord.com/api/webhooks/yyyyyyyyyy/yyyyyyyyyyyyyyyy

---

### 📌 Nota

Esta versión ha sido verificada y es apta para servidores en producción.  
Por favor, reportá cualquier problema o sugerencia en el apartado de Issues del repositorio.

---
