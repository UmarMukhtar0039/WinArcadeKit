add_executable(Sandbox
SandboxApp.cpp
GameStates/TestState.h
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

# Copy assets/ next to the Sandbox exe after every build
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Sandbox>/assets"
    COMMENT "Copying Sandbox assets -> $<TARGET_FILE_DIR:Sandbox>/assets"
)

set_target_properties(Sandbox PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Sandbox>"
)

