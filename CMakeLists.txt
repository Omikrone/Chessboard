cmake_minimum_required(VERSION 3.15)
project(chess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Release build type per default
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

# ----------------------------
# OS detection
# ----------------------------
if(WIN32)
    message(STATUS "Compiling on Windows")
    set(PLATFORM_WINDOWS TRUE)
elseif(UNIX)
    message(STATUS "Compiling on Linux/Mac")
    set(PLATFORM_LINUX TRUE)
endif()

# ----------------------------
# Sources and executable
# ----------------------------
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(chess STATIC ${SOURCES})

# ----------------------------
# Compiling flags
# ----------------------------
if(MSVC)
    add_compile_definitions(_WIN32_WINNT=0x0A00)
    target_compile_options(chess PRIVATE /W4)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -pthread")
endif()

# Include directory
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src)
target_include_directories(chess PRIVATE 
    ${PROJECT_INCLUDE_DIR}
)

# ----------------------------
# Windows specific options
# ----------------------------
if(PLATFORM_WINDOWS)
    # If using vcpkg
    if(DEFINED ENV{VCPKG_ROOT})
        message(STATUS "VCPKG_ROOT detected: $ENV{VCPKG_ROOT}")
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    endif()
endif()
