#include "scanner.hpp"
#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

Scanner::Scanner() {
    logger.info("Scanner initialized");
}

Scanner::~Scanner() {
    logger.info("Scanner destroyed");
}

bool Scanner::initialize() {
    logger.info("Initializing scanner");
    return loadSignatures();
}

bool Scanner::scanFile(const std::string& filePath) {
    if (!Utils::fileExists(filePath)) {
        lastResult = "File not found: " + filePath;
        logger.error(lastResult);
        return false;
    }

    logger.info("Scanning file: " + filePath);
    
    // Calculate file hash
    std::string fileHash = calculateFileHash(filePath);
    
    // Check if hash is in our signatures database
    if (signatures.find(fileHash) != signatures.end()) {
        lastResult = "Malware detected in file: " + filePath + " (Signature match)";
        logger.warning(lastResult);
        return true;
    }
    
    // Perform pattern-based detection
    if (containsMaliciousPatterns(filePath)) {
        lastResult = "Suspicious patterns detected in file: " + filePath;
        logger.warning(lastResult);
        return true;
    }
    
    lastResult = "No threats detected in file: " + filePath;
    logger.info(lastResult);
    return false;
}

int Scanner::scanDirectory(const std::string& dirPath) {
    if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        lastResult = "Directory not found or not accessible: " + dirPath;
        logger.error(lastResult);
        return -1;
    }

    logger.info("Scanning directory: " + dirPath);
    int threatsFound = 0;
    
    for (const auto& entry : fs::recursive_directory_iterator(dirPath)) {
        if (fs::is_regular_file(entry.path())) {
            if (scanFile(entry.path().string())) {
                threatsFound++;
            }
        }
    }
    
    std::stringstream ss;
    ss << "Directory scan completed. Found " << threatsFound << " threats in " << dirPath;
    lastResult = ss.str();
    logger.info(lastResult);
    
    return threatsFound;
}

std::string Scanner::getLastResult() const {
    return lastResult;
}

bool Scanner::loadSignatures(const std::string& signaturesPath) {
    if (!Utils::fileExists(signaturesPath)) {
        logger.error("Signatures file not found: " + signaturesPath);
        return false;
    }
    
    std::ifstream file(signaturesPath);
    if (!file.is_open()) {
        logger.error("Failed to open signatures file: " + signaturesPath);
        return false;
    }
    
    signatures.clear();
    std::string line;
    while (std::getline(file, line)) {
        line = Utils::trim(line);
        if (!line.empty() && line[0] != '#') {  // Skip comments and empty lines
            signatures.insert(line);
        }
    }
    
    file.close();
    
    std::stringstream ss;
    ss << "Loaded " << signatures.size() << " signatures from " << signaturesPath;
    logger.info(ss.str());
    
    return true;
}

std::string Scanner::calculateFileHash(const std::string& filePath) {
    std::string content = Utils::readFileContent(filePath);
    return Utils::calculateSHA256(content);
}

bool Scanner::containsMaliciousPatterns(const std::string& filePath) {
    std::string content = Utils::readFileContent(filePath);
    // Convert to lowercase for case-insensitive matching
    std::string lowerContent = content;
    std::transform(lowerContent.begin(), lowerContent.end(), lowerContent.begin(), ::tolower);
    
    // Check for common malicious patterns
    // This is a simplified example and should be expanded in a real implementation
    const std::vector<std::string> patterns = {
        "virus", "malware", "trojan", 
        "hack", "exploit", "shell_exec", 
        "eval(base64_decode", "system(", "exec(",
        "<script>evil", "cmd.exe /c"
    };
    
    for (const auto& pattern : patterns) {
        if (lowerContent.find(pattern) != std::string::npos) {
            logger.warning("Detected malicious pattern: " + pattern + " in " + filePath);
            return true;
        }
    }
    
    return false;
}