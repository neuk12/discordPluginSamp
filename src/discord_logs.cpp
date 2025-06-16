#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")


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
    } else {
        // printf("WebhookURL [%s] cargado: %s\n", canal, buffer);
    }
}


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


void SendDiscordLog(const char* canal, const char* message)
{
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

    const char* json_format = "{\"content\":\"%s\"}";
    char post_data[2048];
    snprintf(post_data, sizeof(post_data), json_format, message);

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

    HINTERNET hConnect = WinHttpConnect(hSession, whost, INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        printf("Error WinHttpConnect\n");
        WinHttpCloseHandle(hSession);
        return;
    }

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

    const wchar_t* headers = L"Content-Type: application/json";
    BOOL res = WinHttpSendRequest(hRequest, headers, -1,
                                  (LPVOID)post_data, (DWORD)strlen(post_data),
                                  (DWORD)strlen(post_data), 0);
    if (!res) {
        printf("Error WinHttpSendRequest\n");
    } else {
        WinHttpReceiveResponse(hRequest, NULL);
        // printf("Mensaje enviado a canal [%s]\n", canal);
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
}
