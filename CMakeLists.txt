cmake_minimum_required(VERSION 3.15)
project(chess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ----------------------------
# Sources de la librairie
# ----------------------------
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Création de la lib statique
add_library(chess STATIC ${SOURCES})

# Options de compilation
if(MSVC)
    add_compile_definitions(_WIN32_WINNT=0x0A00)
    target_compile_options(chess PRIVATE /W4)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -pthread")
endif()

# Includes
target_include_directories(chess PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)

# ----------------------------
# Windows specific options
# ----------------------------
if(WIN32)
    if(DEFINED ENV{VCPKG_ROOT})
        message(STATUS "VCPKG_ROOT detected: $ENV{VCPKG_ROOT}")
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    endif()
endif()
