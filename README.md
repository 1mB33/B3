# Table of Contents
---
- [About the project](#b3)
- [About demo](#about-demo)
- [Physics showcase](#demos-physics-showcase)
- [Screenshots](#screenshots)
- [Demo keybinds](#current-demo-keybinds)
- [Building the project](#building-the-project)
---

# B3

This project is part of the [BEng Thesis "Design and Implementation of a Game Engine Using Vulkan Technology"](https://github.com/1mB33/BachelorsThesis).
The engine is separated into several modules.
It implements a custom cross-platform window management system with runtime-switchable behavior.
It also features a low-latency input system and a Vulkan-based renderer.

### Engine libraries:
Core:
- testing framework
- debugging tools (logger, assertions)
- exception definitions
- runnable scripts
- synchronization classes (time synchronization)
- compiler attribute macros
- unknown-class definition for indexing classes
- includes of OS, C, and C++ headers
(an STL implementation will be introduced in the future)

Application:
- window management abstraction layer over WinAPI, X11, and Cocoa, created using the policy pattern (strategy pattern)
- input management
- application status management, with exit condition declared via the AppStatus class

Math:
- matrices and vector implementations
- C++ operators for vector math
- geometric primitives

Rendering:
- Vulkan C++ wrappers
- renderer
- swapchain
- pipeline interface
- voxel raytracing pipeline
- sprite rendering pipeline
- shaders

System:
- relations between engine components
- job system
- event system
- reflection system (class names only, with the ability to create objects by requesting them via class name)
- different entry points for the application per operating system

### Currently supported operating systems:
- Windows
- Linux
- macOS (basic support through MoltenVK)

### Voxel raytracing pipeline
For rendering, it uses a voxel traversal algorithm implementation based on
"A Fast Voxel Traversal Algorithm for Ray Tracing"
by John Amanatides and Andrew Woo (1987), and Blinn-Phong reflection lighting.

The pipeline works entirely in a compute shader.


# About demo

In demo we can play a simple minecraft lookalike game where you can place and remove blocks. 
Uses Jolt Physics to showcase the project and test performance.


# Demos physics showcase
<p float="left">
  <img src="https://github.com/1mb33/B3/blob/master/Docs/PROMO_1.gif?raw=true" alt="Screenshot" width="98%"/>
</p>
<p float="left">
  <img src="https://github.com/1mb33/B3/blob/master/Docs/PROMO_2.gif?raw=true" alt="Screenshot" width="98%"/>
</p>

# Screenshots
<p float="left">
  <img src="https://github.com/1mb33/B3/blob/master/Docs/DocScreen01.jpg?raw=true" alt="Screenshot" width="49%"/>
  <img src="https://github.com/1mb33/B3/blob/master/Docs/DocScreen02.jpg?raw=true" alt="Screenshot" width="49%"/>
</p>
<p float="left">
  <img src="https://github.com/1mb33/B3/blob/master/Docs/DocScreen03.jpg?raw=true" alt="Screenshot" width="49%"/>
  <img src="https://github.com/1mb33/B3/blob/master/Docs/readmeSrceenshot_6.jpg?raw=true" alt="Screenshot" width="49%"/>
</p>
<p float="left">
  <img src="https://github.com/1mb33/B3/blob/master/Docs/DocScreen05.jpg?raw=true" alt="Screenshot" width="49%"/>
  <img src="https://github.com/1mb33/B3/blob/master/Docs/DocScreen06.jpg?raw=true" alt="Screenshot" width="49%"/>
</p>

# Current demo keybinds:

### Movement:
W - Move forward<br>
S - Move backward<br>
D - Strafe right<br>
A - Strafe left<br>
E - Fly up<br>
Q - Fly down<br>
Mouse movement - camera rotation<br>

### Interactions:
SPACE or LEFT MOUSE BUTTON - Place a block<br>
X or RIGHT MOUSE BUTTON - Remove a block<br>
1 - Weak push<br>
2 - Medium push<br>
3 - Strong push<br>
Z - Debug view<br>


# Building the project

Build dependencies:
  - Vulkan SDK
  - CMake or Visual Studio with CMake support
  - Compiler that supports at least C++20
  - Windows or Linux operating system
  - X11 libraries (linux only)

## Linux

### Get necessary libraries and headers

### Arch linux based systems
``` sh
sudo pacman -S --needed cmake make libx11 libxext libgcc libxi vulkan-tools vulkan-icd-loader vulkan-headers vulkan-utility-libraries glslang spirv-tools directx-shader-compiler
```

### Ubuntu/Debian based systems
``` sh
sudo apt install -y cmake make libx11-dev libxext-dev gcc libxi-dev vulkan-tools libvulkan-dev libvulkan1 glslang-tools spirv-tools
```

### Download and build the project

``` sh
git clone --recurse-submodules -j8 https://github.com/1mb33/B3.git &&
cd B3 &&
mkdir Build &&
cd Build &&
cmake .. -DCMAKE_BUILD_TYPE=Release &&
cmake --build .
```

Run project from a bin directory that is created in root directory of the project **Bin/TARGET_ARCH/BUILD_TYPE/**.

## Windows

Download Vulkan SDK from a place like [LunarG](https://vulkan.lunarg.com/sdk/home).
If you are using Visual Studio make sure to have CMake support installed.

### Visual Studio

Open root directory of the project with Visual Studio. Wait for cmake to build. Select AtlBee project as a build target.
Prefer release build.

### CMake

``` batch
mkdir Build
cd Build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Run project from a bin directory that is created in root directory of the project **Bin/TARGET_ARCH/BUILD_TYPE/**.
