#include "monitor.hpp"
#include <iostream>
#include <chrono>
#include <map>

Monitor::Monitor() : running(false) {
    logger.info("Monitor initialized");
}

Monitor::~Monitor() {
    if (running) {
        stop();
    }
    logger.info("Monitor destroyed");
}

bool Monitor::start() {
    if (running) {
        logger.warning("Monitor is already running");
        return false;
    }
    
    // Initialize scanner
    if (!scanner.initialize()) {
        logger.error("Failed to initialize scanner");
        return false;
    }
    
    logger.info("Starting real-time monitoring");
    running = true;
    monitorThread = std::thread(&Monitor::monitorFunction, this);
    
    return true;
}

void Monitor::stop() {
    if (!running) {
        logger.warning("Monitor is not running");
        return;
    }
    
    logger.info("Stopping real-time monitoring");
    running = false;
    
    if (monitorThread.joinable()) {
        monitorThread.join();
    }
    
    logger.info("Real-time monitoring stopped");
}

void Monitor::addWatchDirectory(const std::string& directory) {
    std::lock_guard<std::mutex> lock(watchMutex);
    watchDirectories.push_back(directory);
    logger.info("Added directory to watch list: " + directory);
}

bool Monitor::isRunning() const {
    return running;
}

void Monitor::monitorFunction() {
    logger.info("Monitor thread started");
    
    // Store the last modified time of each watched file
    std::map<std::string, fs::file_time_type> lastModifiedMap;
    
    while (running) {
        checkForChanges();
        
        // Sleep to avoid high CPU usage
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    logger.info("Monitor thread stopped");
}

void Monitor::checkForChanges() {
    std::lock_guard<std::mutex> lock(watchMutex);
    
    // Static map to store previous modification times
    static std::map<std::string, fs::file_time_type> lastModifiedMap;
    
    for (const auto& dir : watchDirectories) {
        if (!fs::exists(dir)) {
            logger.warning("Watch directory does not exist: " + dir);
            continue;
        }
        
        try {
            for (const auto& entry : fs::recursive_directory_iterator(dir)) {
                if (!fs::is_regular_file(entry.path())) {
                    continue;
                }
                
                std::string path = entry.path().string();
                auto lastWriteTime = fs::last_write_time(entry.path());
                
                // If the file is new or has been modified
                if (lastModifiedMap.find(path) == lastModifiedMap.end() || 
                    lastModifiedMap[path] != lastWriteTime) {
                    
                    logger.info("Detected file change: " + path);
                    scanner.scanFile(path);
                    
                    // Update the last modified time
                    lastModifiedMap[path] = lastWriteTime;
                }
            }
        } catch (const fs::filesystem_error& e) {
            logger.error("Filesystem error: " + std::string(e.what()));
        }
    }
}