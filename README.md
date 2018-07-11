<img src="https://raw.githubusercontent.com/Equilibrium-Games/Flounder/master/Documents/Flounder.png" alt="Flounder" height="128px">

# Flounder v0.9

[![Contribute!](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/Equilibrium-Games/Flounder/issues)

Flounder is a high speed, modular, light game framework written in C++17 providing multiple features such as cross-platform support.

The Vulkan API is used as the only rendering API; therefore, Vulkan structs and commands can be used directly in games. Flounder allows for direct usage of Vulkan API calls but provides more than enough function to where this type of usage can be avoided.

Features are broken down into modules, modules can be added or removed from Flounder with ease, and modules are easy to create. Vulkan is the only supported rendering API; however, APIs like Molten are supported through libraries, eventually, OpenGL and DirectX will be supported in this way.

Flounder is licensed under MIT, we are open to contributions use the 'Developer Setup' section in the README to get started with Flounder, and if you want to know about our code style read GUIDELINES.

## Builds
Windows   [![Build status](https://ci.appveyor.com/api/projects/status/4uhakf6tt78wov7o?svg=true)](https://ci.appveyor.com/project/Mattparks/flounder)

Linux/MacOS   [![Build Status](https://travis-ci.org/Equilibrium-Games/Flounder.svg?branch=master)](https://travis-ci.org/Equilibrium-Games/Flounder)

# Code snippets
```cpp
// Imports a 2D texture.
auto guiBlack = Texture::Resource("Guis/Black.png");

// Imports a 3D cubemap.
auto skyboxSnowy = Cubemap::Resource("Objects/SkyboxSnowy", ".png");

// Imports a model.
auto dragon = Model::Resource("Objects/Testing/ModelDragon.obj");

// Plays a 3D sound, at the origin, at half volume.
auto jump = Sound("Sounds/Jump.ogg", 0.5f);
jump.SetPosition(Vector3::ZERO);
jump.Play();

// Imports a game object.
auto playerObject = new GameObject("Objects/Player/Player.json", Transform());

// Creates a game object.
auto sphere = new GameObject(Transform(Vector3(6.7f, 6.7f, -8.0f), Vector3::ZERO, 3.0f));
sphere->AddComponent<Mesh>(ShapeSphere::Resource(30, 30, 1.0f));
sphere->AddComponent<MaterialDefault>(Colour::WHITE, Texture::Resource("Objects/Testing/Diffuse.png"),
    0.0f, 0.0f, Texture::Resource("Objects/Testing/Material.png"), Texture::Resource("Objects/Testing/Normal.png"));
sphere->AddComponent<MeshRender>();

// Vector maths.
Vector2 a(3.0f, -7.2f);
Vector2 b(-1.74f, 15.4f);
Vector2 c = a * b;
float dist = a.Distance(b);
```

## Links
Trello   [Board](https://trello.com/b/ZRvpbbYC/flounder)

Readme   [Read more](https://equilibrium-games.github.io/Flounder/)

Documentation   [Autogen docs](https://equilibrium-games.github.io/FlounderDocs/)

Metrics   [Code metrics](https://equilibrium-games.github.io/FlounderDocs/loc/LocMetrics.html)

Website   [Learn more](https://equilibrium.games)

## Platforms
Flounder currently can be run on the following platforms:
 * Vulkan - Windows, Linux, Android
 * MoltenVK - MacOS, IOS

## Features
This is a list of current features in Flounder:
 * On the fly GLSL to SPIR-V compiler
 * Modular rendering pipeline
 * Deferred PBR+IBL rendering
 * Bullet physics engine
 * Frustum and Occlusion culling
 * Resource management
 * Event and tasks systems
 * Resource path searches, and packing
 * GUI and font rendering
 * Particle effect systems
 * Audio and music
 * Skyboxes and fog
 * Shadow mapping
 * GameObjects and component system
 * Post effects (Lensflare, Glow, Blur, SSAO, ...)
 * Model file loading (OBJ)
 * Animations loading (DAE => JSON)
 * Image file loading (JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC)
 * Config and class loading/saving (CSV, JSON)
 * GameObject prefab loading/saving (JSON)
 * Visual drivers and maths classes
 * Flexible input classes
 * C# generated files
 * Networking (WIP)
 * Resource packaging (WIP)
 * Steam integration (WIP

## Dependencies
Flounder uses the following libraries:
 * [Vulkan](https://www.khronos.org/vulkan/) - Vulkan interface
 * [VWSI](https://github.com/mattparks/vwsi) - Window system integration
 * [Glslang](https://github.com/KhronosGroup/glslang) - Shader compiling
 * [OpenAL](http://kcat.strangesoft.net/openal.html) - Audio interface
 * [STB](https://github.com/nothings/stb) - Texture and OGG loading
 * [Bullet3](https://github.com/bulletphysics/bullet3) - Physics integration

## Screenshots
<img src="https://raw.githubusercontent.com/Equilibrium-Games/Flounder/master/Documents/Screenshot1.png" alt="flounder" width="600px">

<img src="https://raw.githubusercontent.com/Equilibrium-Games/Flounder/master/Documents/Screenshot2.png" alt="flounder" width="600px">

<img src="https://raw.githubusercontent.com/Equilibrium-Games/Flounder/master/Documents/Screenshot3.png" alt="flounder" width="600px">

<img src="https://raw.githubusercontent.com/Equilibrium-Games/Flounder/master/Documents/Screenshot4.png" alt="flounder" width="600px">

## License
Flounder is released under the MIT licence, [LICENSE.md](LICENSE.md) for more details.

## Developer Setup
Once Flounder is cloned run `git submodule update --init --recursive` to update the submodules needed to compile. All platforms depend on CMake to generate IDE/make files.

The [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) and [OpenAL SDK](https://www.openal.org/downloads/) is required to develop and run Flounder.

On Windows, Flounder requires Visual Studios 2015 or later and the "Windows 10 SDK (10.0.15063.0) for UWP: C++". Use the VS installer and ensure "Desktop development with C++" and the Windows SDK is installed and up to date. Install the latest [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) and Flounder should compile properly. Run Scripts/generate_vs.bat and an environment will be setup in Build.

Ensure you are using a compiler with full c++17 support, on Windows we recommend using MSVC or [MinGW w64](https://sourceforge.net/projects/mingw-w64/?source=navbar).

On Linux a GCC/Clang compiler is required, each package listed here is for Debian/Apt. Flounder requires `xorg-dev`, `libopenal1`, `libglfw3`, and `libvulkan` to be available.

Setup on MacOS is similar to the setup on Linux, except MoltenVK is used instead of Vulkan.

Currently Flounder does not run under Release mode in Visual Studios (bug).

## Resources
Once CMake has loaded, link the Resources folder into the output directory using `Scripts/link_resources.bat` or `.sh`.

Old resources have been removed from the main repo, older resources can be found on this fork: [https://github.com/mattparks/Flounder](https://github.com/mattparks/Flounder).

## Contributing
You can contribute to Flounder in any way you want, we are always looking for help.
