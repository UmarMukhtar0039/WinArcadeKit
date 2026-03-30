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

