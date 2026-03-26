cmake_minimum_required(VERSION 3.20)
project(WinArcadeKit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# WinArcadeKit static library
add_library(WinArcadeKit STATIC
src/Application.cpp
src/AppWindow.cpp
src/Keyboard.cpp
src/Graphics.cpp
src/Shaders/VertexShader.hlsl
src/Shaders/PixelShader.hlsl
)
set_source_files_properties(src/Shaders/VertexShader.hlsl PROPERTIES
    VS_SHADER_TYPE Vertex
    VS_SHADER_MODEL 5.0
    VS_SHADER_ENTRYPOINT main
)
set_source_files_properties(src/Shaders/PixelShader.hlsl PROPERTIES
    VS_SHADER_TYPE Pixel
    VS_SHADER_MODEL 5.0
    VS_SHADER_ENTRYPOINT main
)

target_include_directories(WinArcadeKit
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_options(WinArcadeKit PUBLIC /utf-8)
target_compile_definitions(WinArcadeKit PUBLIC UNICODE _UNICODE)
target_link_libraries(WinArcadeKit PUBLIC d3d11.lib d3dcompiler.lib)

# Sandbox
add_subdirectory(examples/Sandbox)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)
