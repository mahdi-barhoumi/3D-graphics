# 3D Graphics Engine 🔧

**A lightweight, modular C++ 3D engine for learning and experimenting with real-time rendering, physics, and simple scene management.**

---

## Table of Contents

- [Overview](#overview)
- [Quick Start](#quick-start)
- [Build & Run](#build--run)
- [Controls](#controls)
- [Architecture & Key Components](#architecture--key-components)
- [Examples](#examples)
- [Assets](#assets)
- [Dependencies](#dependencies)
- [Contributing & License](#contributing--license)

---

## Overview ✅

![Demo](demo.gif)

This repository implements a small, self-contained 3D engine written in modern C++ (C++26). It includes:

- A custom Entity-Component system (via `Engine::World` and components)
- Windowing and input handling (using GLFW)
- Audio support (using OpenAL)
- Shader, Mesh, and Texture resource management
- A 3D renderer with default and shadow shaders
- Physics simulation with GJK + EPA collision detection
- Dynamic scene with controllable entities, rigid body dynamics, and physics-based interactions

The code is intentionally compact and educational: it focuses on clarity and modularity so you can inspect and modify systems easily.

---

## Quick Start ⚡

Prerequisites:

- A C++26-capable compiler (GCC, Clang, MSVC)
- make (or a build system of your choice)
- OpenGL development headers and libraries (GLEW, GLFW)
- OpenAL for audio support

Notes: This build is Windows-only. Use MSYS2/MinGW or Visual Studio; the makefile links against `opengl32`, `glew`, `glfw`, `openal32`, and other Windows libraries by default.

---

## Build & Run 🔧

Build using the included makefile (recommended):

```bash
# Debug build (useful for development)
make debug

# Release build
make release

# Default build
make
```

The makefile compiles with `-std=c++26` and produces an executable named `Application` (Windows) or `ApplicationDebug` for debug builds. Run it from the repository root so the `assets/` directory is available:

```bash
# Windows
Application.exe
ApplicationDebug.exe  # Debug build
```

Hint: `make debug` adds `-g -O0` for easier debugging. `make release` builds with optimizations (`-O3`, `-march=native`, `-ffast-math`, `-flto`). The makefile also defines flags such as `-DGLEW_STATIC`, `-DSTB_IMAGE_IMPLEMENTATION`, and `-DAL_LIBTYPE_STATIC`.

---

## Controls 🎮

Example controls used by the demo scene (see `src/main.cpp`):

- **Camera movement**: Z (forward), S (backward), D (right), Q (left), Space (up), Ctrl (down)
- **Camera rotation**: Hold right mouse button and move mouse to look around
- **Force application**: Arrow keys (X/Y), Shift/Ctrl (Z-axis) to apply forces to controllable objects
- **Audio trigger**: Left mouse button plays explosion sound

> Note: Key mapping is exposed via `Engine::Key` (see [include/engine/core/input.hpp](include/engine/core/input.hpp)).

---

## Architecture & Key Components 🏗️

Important directories:

- `include/engine/core/` — Public engine headers
- `src/engine/core/`     — Engine implementation
- `assets/`              — Shaders, meshes, textures

Core concepts:
**World & Entities**: `Engine::World` creates and manages entities (handles). Use `World::Create()` and `World::Get(handle)` to add and retrieve components.
- **Components**: `Transform`, `Camera`, `Mesh`, `Texture`, `Physics`, `Collider`, `Audio`, `Controllable`, `Light`, etc.
- **Renderer**: `Engine::Renderer` holds shaders and renders the world with proper lighting and shadows.
- **Solver**: `Engine::Solver` performs GJK/EPA collision detection and rigid body physics integration.
- **Input**: `Engine::Input` processes keyboard and mouse input, with `Controllable` component for entity control.

Key classes (brief):

- `Engine::World` — Entity-component management system
- `Engine::Window` — Window creation, event processing, buffer swapping
- `Engine::Input` — Keyboard and mouse input handling
- `Engine::Camera` — Perspective/orthographic projections, pan/tilt/roll
- `Engine::Mesh` / `Engine::Texture` — Resource wrappers with lazy loading
- `Engine::Shader` — GLSL shader compilation and uniform management
- `Engine::Physics` — Rigid body dynamics with forces and constraints
- `Engine::Collider` — Collision shapes (Box, Sphere, etc.)
- `Engine::Audio` — Sound playback and 3D audio positioning
- `Engine::Controllable` — Component for player-controlled entities
- `Engine::Light` — Lighting support for dynamic scenshes
- `Engine::Physics` — Per-object physical properties and forces

---

## Examples — Scene construction (from `src/main.cpp`) 💡

```cpp
using namespace Engine;

World world;
Window window(1600, 900);
Renderer renderer;
Solver solver;
Input input;

// Create a camera entity
Object camera = world.Create();
camera.Add<Transform>(Vector3(0.0f, -10.0f, 5.0f));
camera.Add<Camera>();
camera.Add<Audio>("./assets/sounds/explosion.ogg");
world.SetActiveCamera(camera);

// Create a ground plane
Object surface = world.Create();
surface.Add<Transform>();
surface.Add<Mesh>("./assets/meshes/surface.obj");
surface.Add<Texture>("./assets/textures/stone.png");
surface.Add<Collider>(BoxCollider(1000.0f, 1000.0f, 0.0f));
surface.Add<Physics>(Physics::Type::Stationary);

// Create a controllable cube
Object cube = world.Create();
cube.Add<Transform>(Vector3(5.0f, 5.0f, 20.0f), Vector3(2.0f));
cube.Add<Mesh>("./assets/meshes/cube.obj");
cube.Add<Texture>("./assets/textures/dirt.png");
cube.Add<Collider>(BoxCollider(2.0f));
cube.Add<Physics>(4.0f);  // Dynamic with mass
cube.Add<Controllable>();

// Main loop
float deltaTime;
auto now = chrono::high_resolution_clock::now();
while (!window.ShouldClose()) {
  window.ProcessEvents();
  deltaTime = chrono::duration_cast<chrono::nanoseconds>(
    chrono::high_resolution_clock::now() - now).count() * 1e-9;
  now = chrono::high_resolution_clock::now();
  
  input.Control(world, window, deltaTime);
  solver.Solve(world, deltaTime);
  renderer.Render(world, window);
  window.SwapBuffers();
}
```

This example shows how to create entities with multiple components, use physics simulation, handle input, and render a scene.

---

## Assets 📁

- Shaders: `assets/shaders/default/` (vertex/fragment) and `assets/shaders/shadow/` (shadow mapping)
- Meshes: `assets/meshes/*.obj` (examples: `cube.obj`, `sphere.obj`, `surface.obj`)
- Textures: `assets/textures/*.png` (examples: `wood.png`, `dirt.png`, `stone.png`)
- Audio: `assets/sounds/*.ogg` (Vorbis format for OpenAL)

The renderer and audio system expect these files at runtime — make sure the working directory contains the `assets/` folder.

---

## Dependencies & Bundled Libraries 📦

This project bundles several headers and libraries under `include/` and `lib/` to simplify setup:

- **GLFW** — Windowing and input handling
- **GLEW** — OpenGL extension wrangler
- **stb_image** — Image loading (PNG, JPG, etc.)
- **stb_vorbis** — Vorbis audio decoding
- **OpenAL** — 3D audio and sound management (linked from system)

**Linker flags** (Windows, from makefile):
```
-lglew -lglfw -lopengl32 -lopenal32 -lgdi32 -luser32 -lkernel32 -lshell32 -lole32 -luuid -lwinmm -lavrt
```

**Compiler flags** include:
- `-std=c++26` (C++26 standard)
- `-DGLEW_STATIC` (static GLEW linking)
- `-DSTB_IMAGE_IMPLEMENTATION` (STB image loading)
- `-DAL_LIBTYPE_STATIC` (static OpenAL linking)

---

## Notes & Future Improvements 🔭

- **Current scope**: The engine is designed for learning and experimentation with real-time 3D graphics and physics.
- **Performance**: Renderer and physics solver are compact and not optimized for production use. Good for prototyping and educational purposes.
- **Platform**: Currently Windows-only (uses WinAPI for resource handling, OpenAL for audio on Windows).

**Suggested improvements**:
- Multi-platform support (macOS, Linux with cross-platform audio)
- Better resource lifetime tracking and cleanup
- Expanded shader library (normal mapping, parallax mapping, PBR)
- More collision shapes (capsule, convex hull)
- Advanced physics features (constraints, joints)
- Unit tests for collision and physics systems
- CMake support for cleaner builds

---

## Getting Started 🚀

1. **Clone and build**:
   ```bash
   git clone <repo>
   cd 3D\ Graphics
   make debug
   ```

2. **Run the demo**:
   ```bash
   ./ApplicationDebug.exe
   ```

3. **Explore the code**:
   - Start with [src/main.cpp](src/main.cpp) to see scene setup
   - Check [include/engine/core/world.hpp](include/engine/core/world.hpp) for entity management
   - Look at [include/engine/core/solver.hpp](include/engine/core/solver.hpp) for physics logic

4. **Modify the scene**: Edit `src/main.cpp` to add/remove entities and components, then rebuild with `make debug`.
