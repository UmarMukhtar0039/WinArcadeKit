cmake_minimum_required(VERSION 3.20)
project(WinArcadeKit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# DirectXTK — disable features we don't need
set(BUILD_TOOLS OFF CACHE BOOL "" FORCE)           # xwbtool
set(BUILD_XAUDIO_WIN10 OFF CACHE BOOL "" FORCE)    # audio
set(BUILD_XAUDIO_WIN8 OFF CACHE BOOL "" FORCE)     # audio
set(BUILD_XAUDIO_REDIST OFF CACHE BOOL "" FORCE)   # audio
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)          # CTest targets (Continuous, Nightly, etc.)
add_subdirectory(extern/DirectXTK)

# WinArcadeKit static library
add_library(WinArcadeKit STATIC
    src/Application.cpp
    src/AppWindow.cpp
)
target_include_directories(WinArcadeKit
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(WinArcadeKit PUBLIC DirectXTK d3d11.lib d3dcompiler.lib)

add_subdirectory(examples/Sandbox)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)
