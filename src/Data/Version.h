#pragma once
#include <iostream>
#include <string>
#include <regex>

#include <windows.h>
#include <winhttp.h>
#include <sstream>
#include "json.hpp" // Include the JSON library header (https://github.com/nlohmann/json)

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "winhttp.lib")

std::string parseLatestVersion(const std::string& jsonResponse);


class Version
{
private:
    int major;
    int minor;
    int patch;
    bool isAlpha;
public:
    enum Status 
    {
        New,
        OK,
        CantCheck
    };
    Version(const std::string& versionStr)
    {
        std::regex versionRegex("V(\\d+)\\.(\\d+)\\.(\\d+)(a?)");
        std::smatch match;
        if (std::regex_match(versionStr, match, versionRegex))
        {
            major = std::stoi(match[1].str());
            minor = std::stoi(match[2].str());
            patch = std::stoi(match[3].str());
            isAlpha = !match[4].str().empty();
        }
        else
        {
            throw std::invalid_argument("Invalid version string format");
        }
    }

    bool operator<(const Version& other) const
    {
        if (major != other.major) return major < other.major;
        if (minor != other.minor) return minor < other.minor;
        if (patch != other.patch) return patch < other.patch;
        return isAlpha && !other.isAlpha;
    }

    bool operator>(const Version& other) const
    {
        return other < *this;
    }

    bool operator==(const Version& other) const
    {
        return major == other.major && minor == other.minor && patch == other.patch && isAlpha == other.isAlpha;
    }

    bool operator!=(const Version& other) const
    {
        return !(*this == other);
    }

    static std::string CurVersion;
};

std::string GetLatestVersion();