#include <iostream>
#include <string>
#include <vector>
#include "scanner.hpp"
#include "monitor.hpp"
#include "logger.hpp"
#include "utils.hpp"

void printHelp() {
    std::cout << "LiteDefender Antivirus - Simple C++ Antivirus\n";
    std::cout << "Usage:\n";
    std::cout << "  litedefender                      Run interactive mode\n";
    std::cout << "  litedefender --scan <file>        Scan a specific file\n";
    std::cout << "  litedefender --scan-dir <dir>     Scan a directory recursively\n";
    std::cout << "  litedefender --monitor            Enable real-time monitoring\n";
    std::cout << "  litedefender --help               Display this help message\n";
}

void interactiveMode() {
    Scanner scanner;
    Monitor monitor;
    Logger logger;
    
    if (!scanner.initialize()) {
        std::cout << "Failed to initialize scanner. Exiting...\n";
        return;
    }
    
    std::cout << "LiteDefender Antivirus - Interactive Mode\n";
    std::cout << "Type 'help' for a list of commands\n";
    
    while (true) {
        std::cout << "\nLiteDefender> ";
        std::string command;
        std::getline(std::cin, command);
        
        if (command == "help") {
            std::cout << "Available commands:\n";
            std::cout << "  scan <file>       Scan a specific file\n";
            std::cout << "  scandir <dir>     Scan a directory recursively\n";
            std::cout << "  monitor           Toggle real-time monitoring\n";
            std::cout << "  watch <dir>       Add directory to monitoring watchlist\n";
            std::cout << "  status            Show current status\n";
            std::cout << "  quit              Exit the program\n";
        }
        else if (command.substr(0, 5) == "scan ") {
            std::string path = command.substr(5);
            bool result = scanner.scanFile(path);
            std::cout << scanner.getLastResult() << std::endl;
        }
        else if (command.substr(0, 8) == "scandir ") {
            std::string path = command.substr(8);
            scanner.scanDirectory(path);
            std::cout << scanner.getLastResult() << std::endl;
        }
        else if (command == "monitor") {
            if (monitor.isRunning()) {
                monitor.stop();
                std::cout << "Real-time monitoring stopped\n";
            } else {
                if (monitor.start()) {
                    std::cout << "Real-time monitoring started\n";
                } else {
                    std::cout << "Failed to start monitoring\n";
                }
            }
        }
        else if (command.substr(0, 6) == "watch ") {
            std::string path = command.substr(6);
            monitor.addWatchDirectory(path);
            std::cout << "Added " << path << " to watch list\n";
        }
        else if (command == "status") {
            std::cout << "LiteDefender Status:\n";
            std::cout << "  Hostname: " << Utils::getHostname() << "\n";
            std::cout << "  Username: " << Utils::getUsername() << "\n";
            std::cout << "  OS: " << Utils::getOSVersion() << "\n";
            std::cout << "  Monitoring: " << (monitor.isRunning() ? "Active" : "Inactive") << "\n";
        }
        else if (command == "quit" || command == "exit") {
            if (monitor.isRunning()) {
                monitor.stop();
            }
            std::cout << "Exiting LiteDefender. Goodbye!\n";
            break;
        }
        else {
            std::cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        interactiveMode();
        return 0;
    }
    
    // Parse command line arguments
    std::string arg1 = argv[1];
    
    if (arg1 == "--help") {
        printHelp();
        return 0;
    }
    
    Scanner scanner;
    if (!scanner.initialize()) {
        std::cout << "Failed to initialize scanner. Exiting...\n";
        return 1;
    }
    
    if (arg1 == "--scan" && argc > 2) {
        scanner.scanFile(argv[2]);
        std::cout << scanner.getLastResult() << std::endl;
    }
    else if (arg1 == "--scan-dir" && argc > 2) {
        scanner.scanDirectory(argv[2]);
        std::cout << scanner.getLastResult() << std::endl;
    }
    else if (arg1 == "--monitor") {
        Monitor monitor;
        if (!monitor.start()) {
            std::cout << "Failed to start monitoring. Exiting...\n";
            return 1;
        }
        
        std::cout << "Real-time monitoring active. Press Ctrl+C to stop.\n";
        
        // Keep the program running
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    else {
        std::cout << "Invalid command. Use --help for usage information.\n";
        return 1;
    }
    
    return 0;
}