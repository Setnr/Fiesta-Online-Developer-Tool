#pragma once
#include "Version.h"
std::string Version::CurVersion = "V0.0.3";

std::string parseLatestVersion(const std::string& jsonResponse)
{
    auto json = nlohmann::json::parse(jsonResponse);
    if (json.contains("name"))
    {
        return json["name"];
    }
    return "V0.0.0a";
}

std::string GetLatestVersion()
{
    HINTERNET hSession = WinHttpOpen(L"FiestaDeveloperTools VersionRequest",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession)
    {
        return "";
    }

    HINTERNET hConnect = WinHttpConnect(hSession, L"api.github.com",
        INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect)
    {
        WinHttpCloseHandle(hSession);
        return "";
    }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET",
        L"/repos/Setnr/Fiesta-Online-Developer-Tool/releases/latest",
        NULL, WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        WINHTTP_FLAG_SECURE);
    if (!hRequest)
    {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "";
    }

    bool bResults = WinHttpSendRequest(hRequest,
        WINHTTP_NO_ADDITIONAL_HEADERS, 0,
        WINHTTP_NO_REQUEST_DATA, 0,
        0, 0);
    if (bResults)
    {
        bResults = WinHttpReceiveResponse(hRequest, NULL);
    }

    std::string response;
    if (bResults)
    {
        DWORD dwSize = 0;
        WinHttpQueryDataAvailable(hRequest, &dwSize);
        while (dwSize > 0)
        {
            char* buffer = new char[dwSize + 1];
            ZeroMemory(buffer, dwSize + 1);

            DWORD dwDownloaded = 0;
            WinHttpReadData(hRequest, (LPVOID)buffer, dwSize, &dwDownloaded);
            response.append(buffer, dwDownloaded);
            delete[] buffer;

            WinHttpQueryDataAvailable(hRequest, &dwSize);
        }
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return parseLatestVersion(response);
}