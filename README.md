<img src="https://github.com/Equilibrium-Games/Flounder/blob/master/Documents/Flounder.png" alt="flounder" width=225>

# Flounder

[![Contribute!](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/Equilibrium-Games/Flounder/issues)

Flounder is a high speed, modular Vulkan game engine written in C++17 providing multiple features such as cross platform support.

Features are broken down into modules, modules can be added or removed from Flounder with ease, and modules are easy to create. Vulkan is the only supported rendering API; however, APIs like Molten are supported through libraries, eventually, OpenGL and DirectX will be supported in this way..

Flounder is licenced under MIT, we are open to contributions use the 'Developer Setup' section in the README to get started with Flounder, and if you want to know about our code style read GUIDELINES.

## Links
Windows   [![Build status](https://ci.appveyor.com/api/projects/status/4uhakf6tt78wov7o?svg=true)](https://ci.appveyor.com/project/Mattparks/flounder)

Linux   [![Build Status](https://travis-ci.org/Equilibrium-Games/Flounder.svg?branch=master)](https://travis-ci.org/Equilibrium-Games/Flounder)

GitLab   [Issues](https://gitlab.com/mattparks/Flounder/issues)

Website   [Learn more](https://equilibrium.games)

Trello   [Board](https://trello.com/b/ZRvpbbYC/flounder)

Documentation   [Autogen docs](https://equilibrium-games.github.io/Flounder-Docs/docs/html/namespace_flounder.html)

Metrics   [Code metrics](https://equilibrium-games.github.io/Flounder-Docs/loc/LocMetrics.html)

## Features
This is a list of current features in Flounder:
 * High speed, multithreaded, cross platform
 * Only Vulkan rendering
 * On the fly GLSL to SPIR-V compiler
 * Small amount of dependencies
 * Modular rendering pipeline
 * Deferred PBR & IBL rendering
 * Frustum culling
 * Event and tasks systems
 * GUI / text rendering
 * Audio and music
 * Skyboxes and fog
 * Post effects (Lensflare, Glow, Blur, SSAO, ...)
 * Simple terrain, water, and voxels
 * 3D shadow mapping
 * Model file loading (OBJ)
 * Animations loading (DAE => JSON)
 * Image file loading (JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC)
 * Config and class loading/saving (CSV, JSON)
 * Resource management
 * Visual drivers and maths classes
 * Flexible input classes
 * GameObjects and component system
 * GameObject prefab loading/saving (JSON)
 * Material pipeline system
 * Normal, material, glow, and sway maps
 * Particle effects system
 * Simple physics engine (WIP)
 * P2P networking (WIP)
 * Steam integration (WIP)
 * Resource packaging (WIP)

## Plans
These are the future plans for Flounder:
 * Finish the core engine by the end of 2018
 * Create a simple visual prefab editor

## Platforms
Flounder currently can be run on the following platforms:
 * Vulkan - Windows, Linux, Android
 * MoltenVK - MacOS, IOS

## Dependencies
Flounder uses the following libraries:
 * [stb](https://github.com/nothings/stb) - Image loading
 * [glfw](https://github.com/glfw/glfw) - Display handling
 * [glslang](https://github.com/KhronosGroup/glslang) - GLSL processing
 * [openal](http://kcat.strangesoft.net/openal.html) - Audio interface
 * [vulkan](https://www.khronos.org/vulkan/) - Vulkan interface
 * [moltenvk](https://github.com/KhronosGroup/MoltenVK) - Vulkan on Apple

## Screenshots
<img src="https://github.com/Equilibrium-Games/Flounder/blob/master/Documents/Screenshot1.png" alt="flounder" width=600>

<img src="https://github.com/Equilibrium-Games/Flounder/blob/master/Documents/Screenshot2.png" alt="flounder" width=600>

<img src="https://github.com/Equilibrium-Games/Flounder/blob/master/Documents/Screenshot3.png" alt="flounder" width=600>

<img src="https://github.com/Equilibrium-Games/Flounder/blob/master/Documents/Screenshot4.png" alt="flounder" width=600>

## License
Flounder is released under the MIT licence, [LICENSE.md](LICENSE.md) for more details.

## Contributing
You can contribute to Flounder in any way you want, we are always looking for help.

## Developer Setup
Once Flounder is cloned run `git submodule update --init --recursive` to update the submodules needed to compile. All platforms depend on CMake to generate IDE/make files.

The [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) and [OpenAL SDK](https://www.openal.org/downloads/) is required to develop and run Flounder.

On Windows, Flounder requires Visual Studios 2015 or later and the "Windows 10 SDK (10.0.15063.0) for UWP: C++". Use the VS installer and ensure "Desktop development with C++" and the Windows SDK is installed and up to date. Install the latest [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) and Flounder should compile properly. Run Scripts/generate_vs.bat and an environment will be setup in Build.

On Linux a GCC/Clang compiler is required, each package listed here is for Debian/Apt. GLFW requires `xorg-dev`, `libopenal1`, `libglfw3`, and `libvulkan` installed.

Setup on MacOS is similar to the setup on Linux, except MoltenVK is used instead of Vulkan.

Once Flounder has built, link the Resources folder into the output directory. For example when using CLion `Scripts/link_resources.bat` or `.sh` can be used to link into Debug and Release builds.

Currently Flounder does not run under Release mode in Visual Studios (bug).
