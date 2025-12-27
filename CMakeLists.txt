cmake_minimum_required(VERSION 3.16)

project(level_editor
    VERSION 1.0
    LANGUAGES CXX
)

# Standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

# Exécutable
add_executable(level_editor ${SOURCES})

# Includes
target_include_directories(level_editor
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# Warnings
target_compile_options(level_editor
    PRIVATE
        -Wall
        -Wextra
        -Werror
)
