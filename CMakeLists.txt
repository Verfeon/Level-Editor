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
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_demo.cpp
    imgui/backends/imgui_impl_sdl3.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_sdlrenderer3.cpp
)
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/test_imgui.cpp")
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp")

# Exécutable
add_executable(level_editor ${SOURCES} src/main.cpp)

# Includes
target_include_directories(level_editor
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/SDL3-3.2.28/x86_64-w64-mingw32/include
        ${PROJECT_SOURCE_DIR}/imgui
        ${PROJECT_SOURCE_DIR}/imgui/backends
)

# Warnings
target_compile_options(level_editor
    PRIVATE
        -Wall
        -Wextra
)

# Lien vers la bibliothèque SDL3
target_link_libraries(level_editor
    PRIVATE
        ${PROJECT_SOURCE_DIR}/SDL3-3.2.28/x86_64-w64-mingw32/lib/libSDL3.dll.a
)

# Copie de SDL3.dll dans le dossier de build après la compilation
add_custom_command(TARGET level_editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/SDL3-3.2.28/x86_64-w64-mingw32/bin/SDL3.dll"
        $<TARGET_FILE_DIR:level_editor>
)

# Exécutable de test
add_executable(test ${SOURCES}
    src/test_imgui.cpp
)
target_include_directories(test
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/SDL3-3.2.28/x86_64-w64-mingw32/include
        ${PROJECT_SOURCE_DIR}/imgui
        ${PROJECT_SOURCE_DIR}/imgui/backends
)
target_compile_options(test
    PRIVATE
        -Wall
        -Wextra
)
target_link_libraries(test
    PRIVATE
        ${PROJECT_SOURCE_DIR}/SDL3-3.2.28/x86_64-w64-mingw32/lib/libSDL3.dll.a
        opengl32
)
add_custom_command(TARGET test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/SDL3-3.2.28/x86_64-w64-mingw32/bin/SDL3.dll"
        $<TARGET_FILE_DIR:test>
)
