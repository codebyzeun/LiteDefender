// src/utils.cpp
#include "utils.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <functional>
#include <cstring>
#include <filesystem>
#include <array>
#include <random>
#include <limits>

#define NOMINMAX
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/utsname.h>
#endif

namespace fs = std::filesystem;

namespace Utils {
    bool fileExists(const std::string& filePath) {
        return fs::exists(filePath) && fs::is_regular_file(filePath);
    }
    
    std::string readFileContent(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            return "";
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    
    bool writeToFile(const std::string& filePath, const std::string& content) {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            return false;
        }
        
        file << content;
        return !file.fail();
    }
    
    std::vector<std::string> splitString(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        
        return tokens;
    }
    
    std::string trim(const std::string& str) {
        auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char c) {
            return std::isspace(c);
        });
        
        auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char c) {
            return std::isspace(c);
        }).base();
        
        return (start < end) ? std::string(start, end) : std::string();
    }

    // NOTE: Placeholder hash functions. Not for real security use.
    std::string calculateMD5(const std::string& data) {
        std::hash<std::string> hasher;
        size_t hashValue = hasher(data);

        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(32) << hashValue;
        return ss.str();
    }

    std::string calculateSHA1(const std::string& data) {
        std::hash<std::string> hasher;
        size_t hashValue = hasher(data);

        const size_t multiplier = 1099511628211ULL;
        hashValue = (hashValue * multiplier) % std::numeric_limits<size_t>::max();

        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(40) << hashValue;
        return ss.str();
    }

    std::string calculateSHA256(const std::string& data) {
        std::hash<std::string> hasher;
        size_t hashValue = hasher(data);

        const size_t multiplier = 14695981039346656037ULL;
        hashValue = (hashValue * multiplier) % std::numeric_limits<size_t>::max();

        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(64) << hashValue;
        return ss.str();
    }

    std::string getHostname() {
        char hostname[256] = {};

    #ifdef _WIN32
        DWORD size = sizeof(hostname);
        if (GetComputerNameA(hostname, &size) == 0) {
            return "unknown";
        }
    #else
        if (gethostname(hostname, sizeof(hostname)) != 0) {
            return "unknown";
        }
    #endif

        return std::string(hostname);
    }

    std::string getUsername() {
    #ifdef _WIN32
        char username[256] = {};
        DWORD size = sizeof(username);
        if (GetUserNameA(username, &size) == 0) {
            return "unknown";
        }
        return std::string(username);
    #else
        char* username = getenv("USER");
        if (!username) {
            username = getenv("LOGNAME");
        }
        return (username != nullptr) ? std::string(username) : "unknown";
    #endif
    }

    std::string getOSVersion() {
    #ifdef _WIN32
        OSVERSIONINFOA info;
        ZeroMemory(&info, sizeof(OSVERSIONINFOA));
        info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

        if (!GetVersionExA(&info)) {
            return "Windows (version unknown)";
        }

        std::stringstream ss;
        ss << "Windows " << info.dwMajorVersion << "." << info.dwMinorVersion
           << " (Build " << info.dwBuildNumber << ")";
        return ss.str();
    #else
        struct utsname buffer;
        if (uname(&buffer) != 0) {
            return "Unknown UNIX";
        }

        std::stringstream ss;
        ss << buffer.sysname << " " << buffer.release << " " << buffer.version;
        return ss.str();
    #endif
    }
}
