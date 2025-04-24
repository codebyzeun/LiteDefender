# LiteDefender

A lightweight antivirus solution written in C++. LiteDefender is designed to provide basic file scanning and real-time monitoring of your system, helping detect malicious files and patterns based on signatures.

## Features

- **File scanning** using signature-based detection and pattern matching.
- **Real-time system monitoring** to scan files as they are accessed or modified.
- **Comprehensive logging** to track scans, detections, and system information.
- **Cross-platform compatibility** with Windows and Linux support.
- **Simple and intuitive interface** for quick and easy use.

## Building

LiteDefender can be built using CMake and a C++17 compatible compiler. Here's how to set up the project:

1. Clone the repository to your local machine.
    ```bash
    git clone https://github.com/codebyzeun/LiteDefender.git
    cd LiteDefender
    ```

2. Create a build directory and navigate into it:
    ```bash
    mkdir build
    cd build
    ```

3. Generate build files using CMake:
    ```bash
    cmake ..
    ```

4. Build the project using `make` (on Linux) or MSBuild (on Windows):
    ```bash
    make    # On Linux
    cmake --build . --config Release   # On Windows
    ```

## Usage

LiteDefender can be run from the command line. Here are some basic usage instructions:

- **Run the antivirus**: Scan your system for threats.
    ```bash
    ./litedefender
    ```

- **Scan a specific file**: Scan a single file for malware or suspicious patterns.
    ```bash
    ./litedefender --scan /path/to/file
    ```

- **Scan a directory**: Scan an entire directory recursively for malware.
    ```bash
    ./litedefender --scan-dir /path/to/directory
    ```

- **Enable real-time monitoring**: Continuously monitor the system for new threats and scan files as they are accessed or modified.
    ```bash
    ./litedefender --monitor
    ```

- **View logs**: Logs are saved in the `logs` directory (default `litedefender.log`) to track actions taken by LiteDefender.

## Requirements

- **C++17 compatible compiler**
    - GCC 7+ or Clang 5+ for Linux
    - MSVC 2017 or later for Windows
- **CMake 3.10 or higher**

## Configuration

You can customize LiteDefender's behavior by modifying the following files:

- **malware_signatures.txt**: This file contains the malware signatures (hashes) used for file scanning. You can add or remove signatures as needed. By default, LiteDefender includes a basic set of known malware hashes.

- **logging**: By default, all logs are saved to `logs/litedefender.log`. You can adjust logging levels and destinations in the `Logger` class if needed.

## License

LiteDefender is licensed under the MIT License. See the LICENSE file for more details.

## Contributing

We welcome contributions to LiteDefender! If you'd like to contribute, please fork the repository and submit a pull request with your changes. Be sure to test your modifications and update the documentation where necessary.
