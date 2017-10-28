<img src="https://github.com/Equilibrium-Games/Flounder/blob/master/logo.png" alt="flounder" width=225>

# Flounder v0.2
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
 * PBR deferred rendering (WIP)
 * Easy to read documented code
 * Event and tasks systems
 * Component entity system (WIP)
 * 3D audio and music
 * 3D skyboxes
 * 3D terrain and water
 * OBJ model file loading
 * JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC image file loading
 * Visual drivers and maths classes
 * Flexable input classes
 * Networking (WIP)
 * Animations (WIP)
 
Plans (2017):
 * Finish the core engine by the end of
 * A physics engine, ragdoll animations
 * 3D particle effects
 * 3D shadow mapping
 
Maybe:
 * Switch, PlayStation, XBox
 
Preprocessor Definitions:
 * _CRT_SECURE_NO_WARNINGS: Used to ignore 'unsafe' function calls
 * FLOUNDER_PLATFORM_x: Where x is the platform [WINDOWS, LINUX, MACOS, ...]
 * FLOUNDER_VERBOSE: Enables verbose logging
 * FLOUNDER_CONFIG_DEBUG or FLOUNDER_CONFIG_RELEASE: Compiling in release or debug mode

[Read the autogen docs](https://equilibrium-games.github.io/Flounder/Flounder.Core/docs/html/namespace_flounder.html)

[Code metrics](https://equilibrium-games.github.io/Flounder/Flounder.Core/docs/loc/LocMetrics.html)

[Read more on our website](https://equilibrium.games)