#pragma once

#include <string>
#include <mutex>
#include <fstream>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    Logger(const std::string& logFileName = "logs/litedefender.log");
    ~Logger();

    // Log message with specified level
    void log(LogLevel level, const std::string& message);
    
    // Convenience methods for different log levels
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);

private:
    std::string logFileName;
    std::mutex logMutex;
    std::ofstream logFile;
    
    // Convert LogLevel to string
    std::string levelToString(LogLevel level);
    
    // Get current timestamp as string
    std::string getCurrentTimestamp();
};