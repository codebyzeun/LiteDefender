#include <iostream>
#include <cassert>
#include <string>
#include <fstream>
#include "scanner.hpp"
#include "utils.hpp"

// Helper function to create a test file with content
bool createTestFile(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to create test file: " << path << std::endl;
        return false;
    }
    
    file << content;
    file.close();
    return true;
}

// Helper function to create a test signature file
bool createTestSignatureFile(const std::string& path, const std::vector<std::string>& signatures) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to create signature file: " << path << std::endl;
        return false;
    }
    
    for (const auto& sig : signatures) {
        file << sig << std::endl;
    }
    
    file.close();
    return true;
}

void testScanCleanFile() {
    std::cout << "Testing scan of clean file..." << std::endl;
    
    // Create a temporary test file
    std::string testFilePath = "test_clean.txt";
    createTestFile(testFilePath, "This is a clean test file with no malicious content.");
    
    // Create a temporary signature file
    std::string sigFilePath = "signatures/malware_signatures.txt";
    createTestSignatureFile(sigFilePath, {
        Utils::calculateSHA256("Some malicious content"),
        Utils::calculateSHA256("virus code")
    });
    
    Scanner scanner;
    scanner.initialize();
    
    bool result = scanner.scanFile(testFilePath);
    
    // Clean file should return false (no threat detected)
    assert(result == false);
    
    // Clean up
    std::remove(testFilePath.c_str());
    
    std::cout << "Test passed!" << std::endl;
}

void testScanMaliciousFile() {
    std::cout << "Testing scan of malicious file..." << std::endl;
    
    // Create a temporary test file with malicious content
    std::string testFilePath = "test_malicious.txt";
    std::string maliciousContent = "This file contains a virus and should be detected.";
    createTestFile(testFilePath, maliciousContent);
    
    // Create a temporary signature file with the hash of our malicious content
    std::string sigFilePath = "signatures/malware_signatures.txt";
    createTestSignatureFile(sigFilePath, {
        Utils::calculateSHA256(maliciousContent)
    });
    
    Scanner scanner;
    scanner.initialize();
    
    bool result = scanner.scanFile(testFilePath);
    
    // Malicious file should return true (threat detected)
    assert(result == true);
    
    // Clean up
    std::remove(testFilePath.c_str());
    
    std::cout << "Test passed!" << std::endl;
}

void testPatternDetection() {
    std::cout << "Testing pattern-based detection..." << std::endl;
    
    // Create a temporary test file with suspicious patterns
    std::string testFilePath = "test_patterns.txt";
    createTestFile(testFilePath, "This file contains suspicious code like: eval(base64_decode('malicious_code'))");
    
    // Create an empty signature file (we're testing pattern detection, not signature matching)
    std::string sigFilePath = "signatures/malware_signatures.txt";
    createTestSignatureFile(sigFilePath, {});
    
    Scanner scanner;
    scanner.initialize();
    
    bool result = scanner.scanFile(testFilePath);
    
    // File with suspicious patterns should return true
    assert(result == true);
    
    // Clean up
    std::remove(testFilePath.c_str());
    
    std::cout << "Test passed!" << std::endl;
}

void testScanDirectory() {
    std::cout << "Testing directory scanning..." << std::endl;
    
    // Create a test directory with mixed files
    std::string testDirPath = "test_dir";
    std::filesystem::create_directory(testDirPath);
    
    // Create some test files
    createTestFile(testDirPath + "/clean1.txt", "Clean file 1");
    createTestFile(testDirPath + "/clean2.txt", "Clean file 2");
    createTestFile(testDirPath + "/malicious1.txt", "This file has a trojan inside");
    
    // Create signature file
    std::string sigFilePath = "signatures/malware_signatures.txt";
    createTestSignatureFile(sigFilePath, {
        Utils::calculateSHA256("This file has a trojan inside")
    });
    
    Scanner scanner;
    scanner.initialize();
    
    int threatsFound = scanner.scanDirectory(testDirPath);
    
    // Should find 1 threat
    assert(threatsFound == 1);
    
    // Clean up
    std::filesystem::remove_all(testDirPath);
    
    std::cout << "Test passed!" << std::endl;
}

int main() {
    std::cout << "Running scanner tests...\n";
    
    // Create necessary directories
    if (!std::filesystem::exists("signatures")) {
        std::filesystem::create_directory("signatures");
    }
    if (!std::filesystem::exists("logs")) {
        std::filesystem::create_directory("logs");
    }
    
    // Run tests
    testScanCleanFile();
    testScanMaliciousFile();
    testPatternDetection();
    testScanDirectory();
    
    std::cout << "All tests passed!\n";
    return 0;
}