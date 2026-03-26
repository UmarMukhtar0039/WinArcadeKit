add_executable(Sandbox SandboxApp.cpp
GameStates/TestState.cpp   
)
target_link_libraries(Sandbox PRIVATE WinArcadeKit)

# Debug: console window for printf/cerr output
# Release: no console, WAK_DIST switches EntryPoint.h to WinMain
target_link_options(Sandbox PRIVATE
    $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
    $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
)
target_compile_definitions(Sandbox PRIVATE
    $<$<CONFIG:Release>:WAK_DIST>
)

# Set VS debugger working directory to where the exe lives
set_target_properties(Sandbox PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Sandbox>")

# TODO: Temporary copy compiled shaders (.cso) next to the exe.
# Will be replaced by embedding shaders as byte arrays in the .lib later.
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/$<CONFIG>/VertexShader.cso"
        "${CMAKE_BINARY_DIR}/$<CONFIG>/PixelShader.cso"
        "$<TARGET_FILE_DIR:Sandbox>"
)
