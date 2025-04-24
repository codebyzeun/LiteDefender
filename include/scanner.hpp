#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <filesystem>
#include "logger.hpp"

namespace fs = std::filesystem;

class Scanner {
public:
    Scanner();
    ~Scanner();

    // Initialize scanner with signature database
    bool initialize();
    
    // Scan a single file
    bool scanFile(const std::string& filePath);
    
    // Scan entire directory recursively
    int scanDirectory(const std::string& dirPath);
    
    // Get the last scan result
    std::string getLastResult() const;

private:
    Logger logger;
    std::unordered_set<std::string> signatures;
    std::string lastResult;
    
    // Load signatures from file
    bool loadSignatures(const std::string& signaturesPath = "signatures/malware_signatures.txt");
    
    // Calculate file hash for comparison with signatures
    std::string calculateFileHash(const std::string& filePath);
    
    // Check if file content contains known malicious patterns
    bool containsMaliciousPatterns(const std::string& filePath);
};