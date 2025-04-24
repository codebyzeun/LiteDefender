#pragma once

#include <string>
#include <vector>

namespace Utils {
    // File operations
    bool fileExists(const std::string& filePath);
    std::string readFileContent(const std::string& filePath);
    bool writeToFile(const std::string& filePath, const std::string& content);
    
    // String operations
    std::vector<std::string> splitString(const std::string& str, char delimiter);
    std::string trim(const std::string& str);
    
    // Hashing functions
    std::string calculateMD5(const std::string& data);
    std::string calculateSHA1(const std::string& data);
    std::string calculateSHA256(const std::string& data);
    
    // System info
    std::string getHostname();
    std::string getUsername();
    std::string getOSVersion();
}