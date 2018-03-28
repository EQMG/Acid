<img src="https://github.com/Equilibrium-Games/Flounder/blob/master/Documents/Flounder.png" alt="flounder" width=225>

# Flounder v0.8
Flounder is a cross platform high speed C++17 Vulkan game engine. Flounder is built from the ground up to provide the most speed, well written code, and features with a modular engine. Flounder is very much a work in progress, much of this code will change.
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

You will need the [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) and [OpenAL SDK](https://www.openal.org/downloads/) to develop and run Flounder.

For Windows you need Visual Studios 2015 or later, use the VS installer and ensure you have "Desktop development with C++" installed. You should also probably update the Windows SDK, the project is setup for "Windows 10 SDK (10.0.15063.0) for UWP: C++" currently. Install the latest [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) and now Flounder should compile properly. You can now run Scripts/generate_vs.bat and an environment will be setup in Build.

On Linux you need a GCC/Clang compiler, each package listed here are for Debian/Apt. GLFW requires `xorg-dev`, you should also have `libopenal1`, `libglfw3`, and `libvulkan` installed.

Setup on MacOS is similar to the setup on Linux, except MoltenVK is used instead of Vulkan.

Once Flounder has built you need to link the Resources folder into the output directory. For example when using CLion `Scripts/generate_vs.bat` or `.sh` can be used to link into Debug and Release builds.

Currently Flounder does not run under Release mode (IDK why).

## Contributing
You can contribute to Flounder in any way you want, we are always looking for help.
