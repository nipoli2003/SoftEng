cmake_minimum_required(VERSION 3.20)

# Allow dependencies that still specify older CMake minimum versions
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)

project(CardGameSuite VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------
# Dependencies: Raylib
# ---------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# ---------------------------------------------------------
# Model Library (Compiled once, shared with tests & GUI)
# ---------------------------------------------------------
file(GLOB_RECURSE CORE_SOURCES 
    "src/model/*.cpp" 
    "src/controller/*.cpp"
)
add_library(card_model STATIC ${CORE_SOURCES})

target_include_directories(card_model PUBLIC 
    include
)

# ---------------------------------------------------------
# Main Game Executable
# ---------------------------------------------------------
add_executable(card_game 
    src/main.cpp
)

target_include_directories(card_game PRIVATE 
    ${raylib_SOURCE_DIR}/src
)

# Link model + Raylib + platform frameworks
if(APPLE)
    target_link_libraries(card_game PRIVATE 
        card_model
        raylib 
        "-framework OpenGL" 
        "-framework Cocoa" 
        "-framework IOKit" 
        "-framework CoreVideo"
    )
else()
    target_link_libraries(card_game PRIVATE 
        card_model
        raylib
    )
endif()

# ---------------------------------------------------------
# Testing Suite
# ---------------------------------------------------------
enable_testing()
add_subdirectory(tests)