<img src="https://github.com/Equilibrium-Games/Flounder/blob/master/Documents/LogoFlounder.png" alt="flounder" width=225>

# Flounder v0.5
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
 * Windows
 * Linux
 * MacOS (Future)

## Features
 * High speed
 * Cross platform
 * Vulkan rendering
 * Easy to read documented code
 * Event and tasks systems
 * GUI / Text rendering
 * 3D audio and music
 * 3D skybox
 * 3D terrain and water
 * Model file loading (OBJ)
 * Image file loading (JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC)
 * Text file and config loading (CSV, XML)
 * Resource management
 * Visual drivers and maths classes
 * Flexible input classes
 * Component entity system
 * Entity prefab loading
 * Deferred rendering
 * 3D shadow mapping (WIP)
 * Simple physics engine (WIP)
 * P2P networking (WIP)
 * Particle effects (WIP)
 * Animations (WIP)
 * Steam integration (WIP)

## Plans
 * Finish the core engine by the end of 2018 Q2
 * A simple visual editor in the far future

## Developer Setup
You will need the [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) and [OpenAL SDK](https://www.openal.org/downloads/) to develop and run Flounder.

Once Flounder is cloned run `git submodule update --init --recursive` to update the submodules needed to compile.

On all platforms you can compile using CMake.

For Windows you need Visual Studios 2015 or later, use the VS installer and ensure you have "Desktop development with C++" installed. You should also probably update the Windows SDK, the project is setup for "Windows 10 SDK (10.0.15063.0) for UWP: C++" currently. Install the latest [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) and now Flounder should compile properly. You can now run Scripts/generate_vs.bat and an environment will be setup in Build.

## Contributing
You can contribute to Flounder in any way you want, we are always looking for help.