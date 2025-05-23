cmake_minimum_required(VERSION 3.10)
project(AutomotiveSystemManagement)

set(CMAKE_CXX_STANDARD 17)

# Include headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find SQLite3
find_package(SQLite3 REQUIRED)

# Source files
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} main.cpp)

# Link SQLite3
target_link_libraries(${PROJECT_NAME} PRIVATE SQLite::SQLite3)

