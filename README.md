# 3D Graphics Engine

A lightweight, modular C++ 3D graphics project / engine focused on learning and experimenting with real-time rendering concepts using OpenGL.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Repository Structure](#repository-structure)
- [Prerequisites](#prerequisites)
- [Building](#building)
- [Running](#running)
- [Project Layout & Key Components](#project-layout--key-components)

---

## Overview

This project implements a small 3D engine with a focus on clarity and extensibility. The codebase includes a basic renderer, resource management (shaders, textures, meshes), scene/world management, and simple physics and input handling.

The repository contains both source code and vendor headers for common libraries (GLM, stb, GLFW, GLEW) to simplify building and experimentation.

## Features

- Basic scene and object system
- Shader and resource management
- Mesh loading and rendering
- Simple camera and input handling
- Basic physics/collision stubs and utilities
- Example assets (shaders, meshes, textures)

## Repository Structure

Top-level layout (abridged):

```
/ (repo root)
├─ include/            # Public headers and bundled 3rd-party headers
├─ src/                # Implementation sources
├─ assets/             # Shaders, meshes, textures
├─ build/              # Default build directory (CMake / make targets)
├─ tools/              # Utility tools and experiments
├─ makefile            # Simple build entrypoint
└─ README.md           # This file
```

## Prerequisites

- A C++20-compatible compiler (GCC, Clang, MSVC)
- CMake (recommended)
- OpenGL development environment
- Recommended libraries (typically included in `include/`):
  - GLFW
  - GLEW
  - GLM
  - stb_image

On Windows, use MSYS2, Visual Studio, or another environment that supports CMake and your compiler of choice.

> Note: The repository includes copies of several dependencies under `include/` to simplify getting started; system packages can be used as an alternative.

## Building

Recommended (CMake):

```bash
# From project root
cmake -S . -B build
cmake --build build --config Release
```

If the repository provides a `Makefile`, you can also try:

```bash
make
```

The produced executable will be placed in the build output directory. On Windows the executable will typically be under `build/` with a `.exe` extension; on Unix-like systems it will be `build/<executable>`.

## Running

After building, run the produced executable. Example (Unix):

```bash
./build/main
```

On Windows, run the generated `.exe` from the build folder (for example `build\Release\main.exe` or similar depending on the generator and configuration).

The application will load assets from the `assets/` directory (shaders, meshes, textures). Ensure the working directory is set so that `assets/` is accessible at runtime.

## Project Layout & Key Components

- `src/main.cpp` — Application entrypoint
- `include/engine/core/` & `src/engine/core/` — Core engine systems: `window`, `renderer`, `camera`, `world`, `object`, `mesh`, `shader`, `texture`, `input`, `physics`, `light`, etc.
- `include/engine/utils` & `src/engine/utils` — Helper utilities
- `assets/` — Example shaders, meshes, textures used by demos and tests

This organization separates public headers (in `include/`) from implementation (`src/`) to make the core engine reusable and easier to integrate into other projects.
