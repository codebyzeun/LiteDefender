# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(LiteDefender VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/scanner.cpp
    src/monitor.cpp
    src/logger.cpp
    src/utils.cpp
    main.cpp
)

# Create executable
add_executable(litedefender ${SOURCES})

# Create test executable
add_executable(test_scanner tests/test_scanner.cpp src/scanner.cpp src/logger.cpp src/utils.cpp)