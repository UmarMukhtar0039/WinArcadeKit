cmake_minimum_required(VERSION 3.20)
project(WinArcadeKit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compile HLSL shaders to C byte array headers via fxc /Fh
# The generated .h files are #included in Graphics.cpp, baking shader bytecode into the .lib.
set(SHADER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/generated/Shaders)
file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

add_custom_command(
    OUTPUT ${SHADER_OUTPUT_DIR}/VertexShader.h
    COMMAND fxc /nologo /T vs_5_0 /E main /Vn g_VertexShaderBytecode
        /Fh "${SHADER_OUTPUT_DIR}/VertexShader.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/VertexShader.hlsl"
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/VertexShader.hlsl
    COMMENT "Compiling VertexShader.hlsl -> VertexShader.h"
)

add_custom_command(
    OUTPUT ${SHADER_OUTPUT_DIR}/PixelShader.h
    COMMAND fxc /nologo /T ps_5_0 /E main /Vn g_PixelShaderBytecode
        /Fh "${SHADER_OUTPUT_DIR}/PixelShader.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/PixelShader.hlsl"
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/PixelShader.hlsl
    COMMENT "Compiling PixelShader.hlsl -> PixelShader.h"
)

# WinArcadeKit static library
add_library(WinArcadeKit STATIC
src/Application.cpp
src/AppWindow.cpp
src/Keyboard.cpp
src/Graphics.cpp
${SHADER_OUTPUT_DIR}/VertexShader.h
${SHADER_OUTPUT_DIR}/PixelShader.h
)

target_include_directories(WinArcadeKit
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${SHADER_OUTPUT_DIR}
)
target_compile_options(WinArcadeKit PUBLIC /utf-8)
target_compile_definitions(WinArcadeKit PUBLIC UNICODE _UNICODE)
target_link_libraries(WinArcadeKit PUBLIC d3d11.lib)

# Sandbox
add_subdirectory(examples/Sandbox)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)
