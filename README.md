<img src="https://github.com/Equilibrium-Games/Flounder/blob/master/Documents/Flounder.png" alt="flounder" width=225>

# Flounder

[![Contribute!](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/Equilibrium-Games/Flounder/issues)

Flounder is a high speed, modular Vulkan game engine written in C++17 providing multiple features such as cross platform support.

Features are broken down into modules, modules can be added or removed from Flounder with ease, and modules are easy to create. Vulkan is the only supported rendering API; however, APIs like Molten are supported through libraries, eventually, OpenGL and DirectX will be supported in this way.

Flounder is licenced under MIT, we are open to contributions use the 'Developer Setup' section in the README to get started with Flounder, and if you want to know about our code style read GUIDELINES.

## Builds
Windows   [![Build status](https://ci.appveyor.com/api/projects/status/4uhakf6tt78wov7o?svg=true)](https://ci.appveyor.com/project/Mattparks/flounder)

Linux   [![Build Status](https://travis-ci.org/Equilibrium-Games/Flounder.svg?branch=master)](https://travis-ci.org/Equilibrium-Games/Flounder)

## Links
Trello   [Board](https://trello.com/b/ZRvpbbYC/flounder)

Documentation   [Autogen docs](https://equilibrium-games.github.io/Flounder-Docs/docs/html/namespace_flounder.html)

Metrics   [Code metrics](https://equilibrium-games.github.io/Flounder-Docs/loc/LocMetrics.html)

Website   [Learn more](https://equilibrium.games)

## Platforms
Flounder currently can be run on the following platforms:
 * Vulkan - Windows, Linux, Android
 * MoltenVK - MacOS, IOS

## Features
This is a list of current features in Flounder:
 * On the fly GLSL to SPIR-V compiler
 * Modular rendering pipeline
 * Material pipeline system
 * Deferred PBR IBL rendering
 * Normal, material, glow, and sway maps
 * Post effects (Lensflare, Glow, Blur, SSAO, ...)
 * GUI / text rendering
 * Simple terrain, water, and voxels
 * Particle effects system
 * Skyboxes and fog
 * Resource management
 * Audio and music
 * Event and tasks systems
 * Frustum culling
 * Shadow mapping
 * Model file loading (OBJ)
 * Animations loading (DAE => JSON)
 * Image file loading (JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC)
 * Config and class loading/saving (CSV, JSON)
 * GameObject prefab loading/saving (JSON)
 * GameObjects and component system
 * Visual drivers and maths classes
 * Flexible input classes
 * Simple physics engine (WIP)
 * P2P networking (WIP)
 * Steam integration (WIP)
 * Resource packaging (WIP)

## Dependencies
Flounder uses the following libraries:
 * [glfw](https://github.com/glfw/glfw) - Display handling
 * [glslang](https://github.com/KhronosGroup/glslang) - GLSL processing
 * [stb](https://github.com/nothings/stb) - Texture and OGG loading
 * [openal](http://kcat.strangesoft.net/openal.html) - Audio interface
 * [vulkan](https://www.khronos.org/vulkan/) - Vulkan interface
 * [moltenvk](https://github.com/KhronosGroup/MoltenVK) - Vulkan for Apple

## Screenshots
<img src="https://github.com/Equilibrium-Games/Flounder/blob/master/Documents/Screenshot1.png" alt="flounder" width=600>

<img src="https://github.com/Equilibrium-Games/Flounder/blob/master/Documents/Screenshot2.png" alt="flounder" width=600>

<img src="https://github.com/Equilibrium-Games/Flounder/blob/master/Documents/Screenshot3.png" alt="flounder" width=600>

<img src="https://github.com/Equilibrium-Games/Flounder/blob/master/Documents/Screenshot4.png" alt="flounder" width=600>

## License
Flounder is released under the MIT licence, [LICENSE.md](LICENSE.md) for more details.

## Resources
Once CMake has loaded, link the Resources folder into the output directory using `Scripts/link_resources.bat` or `.sh`.

History for `Resources` before https://github.com/Equilibrium-Games/Flounder/tree/e2abd26ee8f3eda4feabbd1ff9b8168f17e8fb4f can be found at: https://github.com/Equilibrium-Games/Flounder-Resources

## Developer Setup
Once Flounder is cloned run `git submodule update --init --recursive` to update the submodules needed to compile. All platforms depend on CMake to generate IDE/make files.

The [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) and [OpenAL SDK](https://www.openal.org/downloads/) is required to develop and run Flounder.

On Windows, Flounder requires Visual Studios 2015 or later and the "Windows 10 SDK (10.0.15063.0) for UWP: C++". Use the VS installer and ensure "Desktop development with C++" and the Windows SDK is installed and up to date. Install the latest [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) and Flounder should compile properly. Run Scripts/generate_vs.bat and an environment will be setup in Build.

On Linux a GCC/Clang compiler is required, each package listed here is for Debian/Apt. GLFW requires `xorg-dev`, `libopenal1`, `libglfw3`, and `libvulkan` installed.

Setup on MacOS is similar to the setup on Linux, except MoltenVK is used instead of Vulkan.

Currently Flounder does not run under Release mode in Visual Studios (bug).

## Contributing
You can contribute to Flounder in any way you want, we are always looking for help.
