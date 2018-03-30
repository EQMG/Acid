<img src="https://github.com/Equilibrium-Games/Flounder/blob/master/Documents/Flounder.png" alt="flounder" width=225>

# Flounder v0.8
Flounder is a high speed, modular Vulkan game engine written in C++17 providing multiple features such as cross platform support.

Flounder is a work in progress, and much of the code is subject to change.

Flounder is licenced under MIT, we are open to contributions use the 'Developer Setup' section in the README to get started with Flounder, and if you want to know about our code style read GUIDELINES.

## Links
Windows   [![Build status](https://ci.appveyor.com/api/projects/status/4uhakf6tt78wov7o?svg=true)](https://ci.appveyor.com/project/Mattparks/flounder)

Linux   [![Build Status](https://travis-ci.org/Equilibrium-Games/Flounder.svg?branch=master)](https://travis-ci.org/Equilibrium-Games/Flounder)


Website   [Learn more](https://equilibrium.games)

Trello   [Board](https://trello.com/b/ZRvpbbYC/flounder)

Documentation   [Autogen docs](https://equilibrium-games.github.io/Flounder-Docs/docs/html/namespace_flounder.html)

Metrics   [Code metrics](https://equilibrium-games.github.io/Flounder-Docs/loc/LocMetrics.html)

## Platforms
 * Vulkan: Windows, Linux, Android
 * MoltenVK: MacOS, IOS
 * D3D12: TODO
 * OpenGL: TODO

## Features
 * High speed
 * Cross platform
 * Only coded for Vulkan rendering
 * On the fly GLSL to SPIR-V compiler
 * Limited amount of libraries
 * Event and tasks systems
 * GUI / text rendering
 * Modular rendering pipeline
 * Deferred rendering
 * Audio and music
 * Skyboxes
 * Large amount of post effects (Lensflare, Glow, Blur)
 * Simple terrain and water
 * Modular voxel system
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
 * Normal, material, glow, and sway maps
 * Particle effects system
 * Simple physics engine (WIP)
 * P2P networking (WIP)
 * Steam integration (WIP)

## Plans
 * Finish the core engine by the end of 2018
 * Allow C# games to be created with Flounder
 * A simple visual editor in the far future

## Developer Setup
Once Flounder is cloned run `git submodule update --init --recursive` to update the submodules needed to compile. All platforms depend on CMake to generate IDE/make files.

The [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) and [OpenAL SDK](https://www.openal.org/downloads/) is required to develop and run Flounder.

On Windows, Flounder requires Visual Studios 2015 or later and the "Windows 10 SDK (10.0.15063.0) for UWP: C++". Use the VS installer and ensure "Desktop development with C++" and the Windows SDK is installed and up to date. Install the latest [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) and Flounder should compile properly. Run Scripts/generate_vs.bat and an environment will be setup in Build.

On Linux a GCC/Clang compiler is required, each package listed here is for Debian/Apt. GLFW requires `xorg-dev`, `libopenal1`, `libglfw3`, and `libvulkan` installed.

Setup on MacOS is similar to the setup on Linux, except MoltenVK is used instead of Vulkan.

Once Flounder has built, link the Resources folder into the output directory. For example when using CLion `Scripts/link_resources.bat` or `.sh` can be used to link into Debug and Release builds.

Currently Flounder does not run under Release mode (IDK why).

## Contributing
You can contribute to Flounder in any way you want, we are always looking for help.
