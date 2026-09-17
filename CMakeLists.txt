cmake_minimum_required(VERSION 3.20)

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
# Model & Controller Library (Model + Controller, Headless)
# ---------------------------------------------------------
file(GLOB_RECURSE MODEL_SOURCES "src/model/*.cpp")
file(GLOB_RECURSE CONTROLLER_SOURCES "src/controller/*.cpp")

add_library(card_model STATIC
    ${MODEL_SOURCES}
    ${CONTROLLER_SOURCES}
)

target_include_directories(card_model PUBLIC 
    include
)

# ---------------------------------------------------------
# Main Game Executable (Links Model + View + Raylib)
# ---------------------------------------------------------
file(GLOB_RECURSE VIEW_SOURCES "src/view/*.cpp")

add_executable(card_game 
    src/main.cpp
    ${VIEW_SOURCES}
)

# Copy assets folder AFTER the card_game target is declared
add_custom_command(TARGET card_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:card_game>/assets
)

target_include_directories(card_game PRIVATE 
    include
    ${raylib_SOURCE_DIR}/src
)

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