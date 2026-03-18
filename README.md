# WinArcadeKit

A minimal 2D game framework for Windows, built on Direct3D 11 and DirectXTK.

## What is this?

WinArcadeKit is a lightweight framework for building 2D arcade-style games on Windows. It handles the boilerplate — windowing, rendering, input, game loop, state management, asset loading — so you can focus on making games.

## Tech Stack

- **Graphics**: Direct3D 11
- **Helpers**: DirectXTK
- **Math**: DirectXMath
- **Windowing**: Win32 API
- **Shaders**: HLSL (Shader Model 5.0)

## Planned Features

- [ ] Game loop with fixed window and viewport setup
- [ ] Game state machine (menu, playing, game over, etc.)
- [ ] Primitive rendering (lines, triangles)
- [ ] Sprite/texture rendering
- [ ] Text rendering
- [x] Keyboard input handling
- [ ] Collision detection
- [ ] Asset management

## Requirements

- Windows 10/11
- CMake 3.20+
- A C++20 compiler (MSVC, Clang-cl, etc.)
- Windows SDK

## Getting Started

1. Add WinArcadeKit as a submodule in your game project:
   ```bash
   git submodule add <repo-url> extern/WinArcadeKit
   git submodule update --init --recursive
   ```
2. In your project's `CMakeLists.txt`:
   ```cmake
   add_subdirectory(extern/WinArcadeKit)
   target_link_libraries(MyGame PRIVATE WinArcadeKit)
   ```
3. Build with CMake:
   ```bash
   cmake -B build
   cmake --build build
   ```

## License

MIT
