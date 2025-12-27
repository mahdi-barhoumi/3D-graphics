# 3D Graphics

Small collection of 3D graphics headers and utilities used by the project:
- GLFW (windowing & input)
- GLEW (OpenGL extension loader)
- GLM (math)
- stb_image (image loading)

Purpose
- Provide the project with common cross-platform graphics libraries and helpers.
- Serve as a convenience copy of headers/source for local development.

Quick start (generic)
1. Install prerequisites:
   - CMake (>= 3.10)
   - A C/C++ compiler (MSVC, clang, or gcc)
   - OpenGL development headers/libraries (system dependent)
2. Create a build directory and run CMake:
   - mkdir build && cd build
   - cmake .. 
   - cmake --build . --config Release
3. Run your executable from the build output. Ensure any required DLLs (e.g. glfw, glew) are on PATH or next to the executable on Windows.

Notes
- This repository contains third-party code (GLFW, GLEW, GLM, stb). Consult the headers for the exact license text. Do not remove or alter license texts from those files.
- If you see missing-linker or unresolved-symbol errors, confirm your linker includes OpenGL and the appropriate platform libraries (e.g. opengl32 on Windows, GL on Linux) and that any dynamic libraries are available at runtime.

Project structure (high level)
- include/ : headers for GLFW, GLEW, GLM, stb and others
- src/ : (if present) project source files
- CMakeLists.txt or other build files at project root (if present)

Troubleshooting
- GL version/context issues: check glfw window hints and ensure drivers support requested GL profile.
- stb_image: When using STB from source, define implementation macro (e.g. `#define STB_IMAGE_IMPLEMENTATION`) in a single translation unit.

Contributing
- Add changes, tests or examples and open a PR. Keep third-party license notices intact.

License
- This repository includes third-party components. See their headers for license details. The project itself does not remove or modify those notices.
