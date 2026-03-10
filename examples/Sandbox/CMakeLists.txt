add_executable(Sandbox SandboxApp.cpp)
target_link_libraries(Sandbox PRIVATE WinArcadeKit)

# Debug: console window for printf/cerr output
target_link_options(Sandbox PRIVATE /SUBSYSTEM:CONSOLE)
