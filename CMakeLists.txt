cmake_minimum_required(VERSION 3.20)
project(WinArcadeKit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# DirectXTK (git submodule)
add_subdirectory(extern/DirectXTK)

# WinArcadeKit static library
# add_library(WinArcadeKit STATIC)
# target_include_directories(WinArcadeKit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
# target_link_libraries(WinArcadeKit PUBLIC DirectXTK d3d11.lib d3dcompiler.lib)
