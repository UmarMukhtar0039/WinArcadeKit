# WinArcadeKit

A minimal 2D game framework for Windows, built from scratch on Direct3D 11.

## What is this?

WinArcadeKit is a lightweight framework for building 2D arcade-style games on Windows. It handles the boilerplate windowing, rendering, input, game loop, state management, asset loading so you can focus on making games.

## Tech Stack

- **Graphics**: Direct3D 11 (raw, no external libraries)
- **Windowing**: Win32 API
- **Shaders**: HLSL (Shader Model 5.0)
- **COM Management**: WRL ComPtr

## Planned Features

- [x] Game loop with window setup
- [x] Game state machine (menu, playing, game over, etc.)
- [x] Keyboard input handling
- [x] D3D11 graphics device (ComPtr, factory pattern, HRESULT checks)
- [ ] Timestep (frame-independent updates)
- [ ] Primitive rendering (lines, triangles via ImmediateMode)
- [ ] Sprite/texture rendering
- [ ] Text/font rendering
- [ ] Audio (XAudio2)
- [ ] Collision detection

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
