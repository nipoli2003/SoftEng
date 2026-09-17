cmake_minimum_required(VERSION 3.20)

# Allow dependencies that still specify older CMake minimum versions
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)

project(CardGameSuite VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Download modern Raylib (master branch or 5.5 has the updated CMake policy)
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
)

# Prevent Raylib from compiling unnecessary examples and tests
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

# Include paths
include_directories(
    include
    ${raylib_SOURCE_DIR}/src
)

# Collect source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Executable target
add_executable(card_game ${SOURCES})

# macOS Frameworks for windowing and rendering
if(APPLE)
    target_link_libraries(card_game PRIVATE 
        raylib 
        "-framework OpenGL" 
        "-framework Cocoa" 
        "-framework IOKit" 
        "-framework CoreVideo"
    )
else()
    target_link_libraries(card_game PRIVATE raylib)
endif()