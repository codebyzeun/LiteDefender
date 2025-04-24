#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include "scanner.hpp"
#include "logger.hpp"

class Monitor {
public:
    Monitor();
    ~Monitor();

    // Start real-time monitoring
    bool start();
    
    // Stop monitoring
    void stop();
    
    // Add directory to watch list
    void addWatchDirectory(const std::string& directory);
    
    // Is monitoring active?
    bool isRunning() const;

private:
    Scanner scanner;
    Logger logger;
    std::thread monitorThread;
    std::atomic<bool> running;
    std::mutex watchMutex;
    std::vector<std::string> watchDirectories;
    
    // Main monitoring function
    void monitorFunction();
    
    // Check for file system changes
    void checkForChanges();
};