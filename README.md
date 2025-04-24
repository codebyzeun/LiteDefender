# LiteDefender

A lightweight antivirus solution written in C++.

## Features

* File scanning using signature-based detection
* Real-time system monitoring
* Comprehensive loggingp
* Simple and intuitive interface

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

```bash
# Run the antivirus
./litedefender

# Scan a specific file
./litedefender --scan /path/to/file

# Scan a directory
./litedefender --scan-dir /path/to/directory

# Enable real-time monitoring
./litedefender --monitor
```

## Requirements

* C++17 compatible compiler
* CMake 3.10 or higher
