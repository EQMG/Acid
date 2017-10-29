<img src="https://github.com/Equilibrium-Games/Flounder/blob/master/logo.png" alt="flounder" width=225>

# Flounder v0.3
[![Build status](https://ci.appveyor.com/api/projects/status/4uhakf6tt78wov7o?svg=true)](https://ci.appveyor.com/project/Mattparks/flounder)

Flounder is a cross platform high speed C++17 Vulkan game engine. Flounder is built from the ground up to provide the most speed, well written code, and features with a modular engine. Flounder is very much a work in progress, much of this code will change.

Libraries:
 * Vulkan
 * GLFW
 * OpenAL
Supported platforms:
 * Windows
 * Linux
 * MacOS

Features:
 * High speed
 * Cross platform
 * Vulkan rendering
 * Easy to read documented code
 * PBR deferred rendering (WIP)
 * Event and tasks systems
 * Component entity system (WIP)
 * GUI / Text rendering
 * 3D audio and music
 * 3D skybox
 * 3D terrain and water
 * Model file loading (OBJ)
 * Image file loading (JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC)
 * Visual drivers and maths classes
 * Flexable input classes
 * Networking (WIP)
 * Animations (WIP)
Plans:
 * 3D shadow mapping
 * 3D particle effects
 * Finish the core engine by the end of 2017
 * A physics engine, ragdoll animations
Maybe:
 * Switch, PlayStation, XBox

Preprocessor Definitions:
 * _CRT_SECURE_NO_WARNINGS: Used to ignore 'unsafe' function calls
 * FLOUNDER_PLATFORM_x: Where x is the platform [WINDOWS, LINUX, MACOS, ...]
 * FLOUNDER_VERBOSE: Enables verbose logging
 * FLOUNDER_CONFIG_DEBUG or FLOUNDER_CONFIG_RELEASE: Compiling in release or debug mode
 
# Developer Setup
For Windows you need Visual Studios 2015 or newer, use the VS installer and ensure you have "Desktop development with C++" installed. You should also probably update the Windows SDK, the project is setup for "Windows 10 SDK (10.0.15063.0) for UWP: C++" currently. Install the latest [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) and now Flounder should compile properly.
TODO: MacOS/Linux files.

# Links
[Read the autogen docs](https://equilibrium-games.github.io/Flounder-Docs/docs/html/namespace_flounder.html)

[Code metrics](https://equilibrium-games.github.io/Flounder-Docs/loc/LocMetrics.html)

[Read more on our website](https://equilibrium.games)
