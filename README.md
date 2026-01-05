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

This repository implements a small, self-contained 3D engine written in modern C++ (C++23). It includes:

- A simple Entity-Component system (backed by entt)
- Windowing and input (GLFW)
- Shader, Mesh and Texture resource management
- A basic renderer with a default shader and a grid shader
- A physics solver using GJK + EPA for collision detection and response
- Example scene construction and input-driven interactions

The code is intentionally compact and educational: it focuses on clarity and modularity so you can inspect and modify systems easily.

---

## Quick Start ⚡

Prerequisites:

- A C++23-capable compiler (GCC, Clang, MSVC)
- make (or a build system of your choice)
- OpenGL development headers and libraries (GLEW, GLFW)

Notes for Windows: Use MSYS2/MinGW or Visual Studio; the Makefile links against `opengl32`, `glew`, and `glfw` by default.

---

## Build & Run 🔧

Build using the included Makefile (recommended):

```bash
# Debug build (useful for development)
make debug

# Release build
make release

# Default build
make
```

The Makefile compiles with `-std=c++23` and produces an executable named `Application` (or `Application.exe` on Windows). Run it from the repository root so the `assets/` directory is available:

```bash
# Unix
./Application
# Windows
Application.exe
```

Hint: `make debug` adds `-g -O0` for easier debugging. The Makefile also defines flags such as `-DGLEW_STATIC` and `-DSTB_IMAGE_IMPLEMENTATION`.

---

## Controls 🎮

Example controls used by the demo scene (see `src/main.cpp`):

- Camera movement: Z (forward), S (backward), D (right), Q (left)
- Camera roll:    E (roll right), A (roll left)
- Mouse movement: Pan / Tilt the camera
- Physics forces: Arrow keys apply forces to physics-enabled objects
  - UpArrow / DownArrow / LeftArrow / RightArrow
  - LeftShift / LeftControl (apply Z-axis forces)

> Note: the key mapping is exposed via `Engine::Key` (see `include/engine/core/input.hpp`).

---

## Architecture & Key Components 🏗️

Important directories:

- `include/engine/core/` — Public engine headers
- `src/engine/core/`     — Engine implementation
- `assets/`              — Shaders, meshes, textures

Core concepts:

- World & Entities: `Engine::World` creates and manages entities (handles). Use `World::Create()` and `World::Get(handle)` to add components.
- Components: `Transform`, `Camera`, `Mesh`, `Texture`, `Physics`, `Input`, etc.
- Renderer: `Engine::Renderer` holds default shaders and exposes `Render(World&, Window&)`.
- Solver: `Engine::Solver` performs collision detection (GJK/EPA) and integrates physics.

Key classes (brief):

- `Engine::Window` — Create windows, process events, swap buffers
- `Engine::Input` — Queues keyboard and mouse movement events
- `Engine::Camera` — FOV, projection matrix, pan/tilt/roll helpers
- `Engine::Mesh` / `Engine::Texture` — Resource wrappers with caching
- `Engine::Shader` — Load/compile GLSL, set uniforms, draw meshes
- `Engine::Physics` — Per-object physical properties and forces

---

## Examples — Minimal scene (inspired by `src/main.cpp`) 💡

```cpp
using namespace Engine;

World world;
Window window("3D", 1280, 720);
Renderer renderer;
Solver solver;

// Create a camera entity
Handle cameraHandle = world.Create();
Object camera = world.Get(cameraHandle);
camera.Add<Transform>(Transform());
camera.Add<Camera>(Camera());
camera.Add<Input>(Input(window));

// Create a textured mesh entity
Handle cubeHandle = world.Create();
Object cube = world.Get(cubeHandle);
cube.Add<Transform>(Transform());
cube.Add<Mesh>(Mesh("./assets/meshes/cube.obj"));
cube.Add<Texture>(Texture("./assets/textures/wood.png"));
cube.Add<Physics>(Physics(CubeCollider(2)));

// Main loop: process events, solve physics, render
while (!window.ShouldClose()) {
  window.ProcessEvents();
  solver.Solve(world, deltaTime);
  renderer.Render(world, window);
  window.SwapBuffers();
}
```

This example highlights how the engine composes entities from components and runs a simple game loop.

---

## Assets 📁

- Shaders: `assets/shaders/default/*` and `assets/shaders/grid/*`
- Meshes: `assets/meshes/*.obj` (examples: `cube.obj`, `surface.obj`)
- Textures: `assets/textures/*.png` (examples: `wood.png`, `dirt.png`, `stone.png`)

The renderer expects these files at runtime — make sure the working directory contains the `assets/` folder.

---

## Dependencies & Bundled Libraries 📦

This project bundles several headers under `include/` to simplify setup:

- GLM (math)
- GLFW (windowing/input)
- GLEW (OpenGL extension wrangler)
- stb_image (image loading)
- entt (entity-component system)

Linker flags in the Makefile: `-lglew -lglfw -lopengl32 -lgdi32 -luser32 -lkernel32` (Windows target shown).

---

## Notes & Next Steps 🔭

- The code is geared toward learning and small experiments; the renderer and physics solver are intentionally compact and not optimized for production.
- Suggested improvements: better resource lifetime tracking, expanded shader examples, more robust build scripts (CMake), and tests for the physics solver.

---

If you'd like, I can:

- Add a short usage example binary in `build/` or a small script to run the app ✅
- Add a `CONTRIBUTING.md` and `LICENSE` file (choose a license) ✅
- Improve the Makefile to support out-of-tree builds and platform detection ✅

Tell me which of the above you'd like me to do next.
