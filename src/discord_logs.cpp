#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <winhttp.h>
#include <string>

#pragma comment(lib, "winhttp.lib")

enum class DiscordColor : unsigned int {
    Verde    = 0x00FF00,   // Verde
    Amarillo = 0xFFFF00,   // Amarillo
    Rojo     = 0xFF0000,   // Rojo
    Azul     = 0x0000FF,   // Azul
    Blanco   = 0xFFFFFF,   // Blanco
    Naranja  = 0xFF9900    // Naranja
};

DiscordColor ColorFromString(const char* colorStr)
{
    if (strcmp(colorStr, "Verde") == 0 || strcmp(colorStr, "Green") == 0) {
        return DiscordColor::Verde;
    } else if (strcmp(colorStr, "Amarillo") == 0 || strcmp(colorStr, "Yellow") == 0) {
        return DiscordColor::Amarillo;
    } else if (strcmp(colorStr, "Rojo") == 0 || strcmp(colorStr, "Red") == 0) {
        return DiscordColor::Rojo;
    } else if (strcmp(colorStr, "Azul") == 0 || strcmp(colorStr, "Blue") == 0) {
        return DiscordColor::Azul;
    } else if (strcmp(colorStr, "Blanco") == 0 || strcmp(colorStr, "White") == 0) {
        return DiscordColor::Blanco;
    } else if (strcmp(colorStr, "Naranja") == 0 || strcmp(colorStr, "Orange") == 0) {
        return DiscordColor::Naranja;
    } else {
        // Color por defecto
        return DiscordColor::Blanco;
    }
}


// Leer URL del webhook desde archivo ini
void LeerWebhookDesdeIni(const char* canal, char* buffer, int bufferSize)
{
    char exePath[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    char* lastSlash = strrchr(exePath, '\\');
    if (lastSlash)
        *lastSlash = '\0';

    char iniPath[MAX_PATH] = {0};
    snprintf(iniPath, MAX_PATH, "%s\\discord.ini", exePath);

    DWORD charsRead = GetPrivateProfileStringA("Discord", canal, "", buffer, bufferSize, iniPath);
    if (charsRead == 0) {
        printf("No se pudo leer webhook para canal [%s] o archivo %s no existe.\n", canal, iniPath);
    }
}

// Parsea URL para separar host y path
int ParseURL(const char* url, char* host, int hostSize, char* path, int pathSize)
{
    const char* prefix = "https://";
    int prefix_len = (int)strlen(prefix);

    if (strncmp(url, prefix, prefix_len) != 0) {
        return 0;
    }

    const char* host_start = url + prefix_len;
    const char* path_start = strchr(host_start, '/');
    if (!path_start) {
        strncpy(host, host_start, hostSize - 1);
        host[hostSize - 1] = 0;
        strncpy(path, "/", pathSize);
        return 1;
    }

    int host_len = (int)(path_start - host_start);
    if (host_len >= hostSize) return 0;

    strncpy(host, host_start, host_len);
    host[host_len] = 0;

    strncpy(path, path_start, pathSize - 1);
    path[pathSize - 1] = 0;

    return 1;
}



bool AnsiToUtf8(const char* input, char* output, size_t outputSize)
{
    if (!input || !output || outputSize == 0)
        return false;

    // Primero convertir ANSI a UTF-16
    int wlen = MultiByteToWideChar(CP_ACP, 0, input, -1, NULL, 0);
    if (wlen == 0)
        return false;

    wchar_t* wbuffer = new wchar_t[wlen];
    if (!wbuffer)
        return false;

    MultiByteToWideChar(CP_ACP, 0, input, -1, wbuffer, wlen);

    // Luego UTF-16 a UTF-8
    int u8len = WideCharToMultiByte(CP_UTF8, 0, wbuffer, -1, NULL, 0, NULL, NULL);
    if (u8len == 0)
    {
        delete[] wbuffer;
        return false;
    }

    if ((size_t)u8len > outputSize)
    {
        delete[] wbuffer;
        return false; // buffer muy pequeño
    }

    WideCharToMultiByte(CP_UTF8, 0, wbuffer, -1, output, (int)outputSize, NULL, NULL);

    delete[] wbuffer;
    return true;
}




void SendDiscordLog(const char* canal, const char* mensajeEscapado, const char* colorNombre, const char* mentionEscapado = nullptr)
{
    DiscordColor color = ColorFromString(colorNombre);
    char webhook_url[1024] = {0};
    LeerWebhookDesdeIni(canal, webhook_url, sizeof(webhook_url));
    if (webhook_url[0] == 0) {
        printf("Error: Webhook URL no configurada para [%s]\n", canal);
        return;
    }

    char host[256] = {0};
    char path[768] = {0};
    if (!ParseURL(webhook_url, host, sizeof(host), path, sizeof(path))) {
        printf("Error: Webhook URL malformada para [%s]\n", canal);
        return;
    }
    char mensajeUtf8[2048];
    if (!AnsiToUtf8(mensajeEscapado, mensajeUtf8, sizeof(mensajeUtf8))) {
        // printf("Error al convertir mensaje a UTF-8\n");
        return;
    }

    char mentionUtf8[512] = "";
    if (mentionEscapado && mentionEscapado[0] != '\0') {
        if (!AnsiToUtf8(mentionEscapado, mentionUtf8, sizeof(mentionUtf8))) {
            // printf("Error al convertir mention a UTF-8\n");
            return;
        }
    }
    // Armar JSON según si hay mention o no
    char post_data[4096];
    if (mentionUtf8[0] != '\0') {
        snprintf(post_data, sizeof(post_data),
                 "{\"content\":\"%s\",\"embeds\":[{\"description\":\"%s\",\"color\":%u}]}",
                 mentionUtf8, mensajeUtf8, static_cast<unsigned int>(color));
    } else {
        snprintf(post_data, sizeof(post_data),
                 "{\"embeds\":[{\"description\":\"%s\",\"color\":%u}]}",
                 mensajeUtf8, static_cast<unsigned int>(color));
    }

    // printf("[DiscordLogs] JSON preparado: %s\n", post_data);

    // Convertir host y path a wchar_t para WinHttp
    wchar_t whost[256];
    wchar_t wpath[768];
    mbstowcs(whost, host, sizeof(whost) / sizeof(wchar_t));
    mbstowcs(wpath, path, sizeof(wpath) / sizeof(wchar_t));

    HINTERNET hSession = WinHttpOpen(L"SA-MP Discord Webhook/1.0",
                                     WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                     WINHTTP_NO_PROXY_NAME,
                                     WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        printf("Error WinHttpOpen\n");
        return;
    }
    // printf("WinHttpOpen OK\n");

    HINTERNET hConnect = WinHttpConnect(hSession, whost, INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        printf("Error WinHttpConnect\n");
        WinHttpCloseHandle(hSession);
        return;
    }
    // printf("WinHttpConnect OK\n");

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", wpath,
                                            NULL, WINHTTP_NO_REFERER,
                                            WINHTTP_DEFAULT_ACCEPT_TYPES,
                                            WINHTTP_FLAG_SECURE);
    if (!hRequest) {
        printf("Error WinHttpOpenRequest\n");
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return;
    }
    // printf("WinHttpOpenRequest OK\n");

    const wchar_t* headers = L"Content-Type: application/json";
    BOOL res = WinHttpSendRequest(hRequest, headers, -1,
                                  (LPVOID)post_data, (DWORD)strlen(post_data),
                                  (DWORD)strlen(post_data), 0);
    if (!res) {
        printf("Error WinHttpSendRequest\n");
    } else {
        // printf("WinHttpSendRequest OK\n");
        if (!WinHttpReceiveResponse(hRequest, NULL)) {
            // printf("Error WinHttpReceiveResponse\n");
        } else {
            DWORD statusCode = 0;
            DWORD statusSize = sizeof(statusCode);
            if (WinHttpQueryHeaders(hRequest,
                                    WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                                    NULL, &statusCode, &statusSize, NULL)) {
                // printf("HTTP status code: %u\n", statusCode);
                if (statusCode != 204) {
                    printf("Error: Discord webhook HTTP %u\n", statusCode);
                } else {
                    // printf("Mensaje enviado exitosamente a Discord\n");
                }
            } else {
                printf("Error WinHttpQueryHeaders\n");
            }
        }
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
}
