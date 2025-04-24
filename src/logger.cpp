#include "logger.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <filesystem>

namespace fs = std::filesystem;

Logger::Logger(const std::string& logFileName) : logFileName(logFileName) {
    // Create logs directory if it doesn't exist
    fs::path logDir = fs::path(logFileName).parent_path();
    if (!logDir.empty() && !fs::exists(logDir)) {
        fs::create_directories(logDir);
    }
    
    // Open log file
    logFile.open(logFileName, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: " << logFileName << std::endl;
    } else {
        logFile << "\n===== LiteDefender started at " << getCurrentTimestamp() << " =====\n";
        logFile.flush();
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile << "===== LiteDefender stopped at " << getCurrentTimestamp() << " =====\n\n";
        logFile.close();
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    std::string timestamp = getCurrentTimestamp();
    std::string levelStr = levelToString(level);
    
    std::stringstream ss;
    ss << timestamp << " [" << levelStr << "] " << message;
    std::string logMessage = ss.str();
    
    // Write to log file
    if (logFile.is_open()) {
        logFile << logMessage << std::endl;
        logFile.flush();
    }
    
    // Also print to console for higher severity levels
    if (level >= LogLevel::WARNING) {
        std::cerr << logMessage << std::endl;
    } else {
        std::cout << logMessage << std::endl;
    }
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";
        default:                return "UNKNOWN";
    }
}

std::string Logger::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    
    return ss.str();
}