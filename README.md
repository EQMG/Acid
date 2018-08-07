<img src="https://raw.githubusercontent.com/Equilibrium-Games/Acid/master/Documents/Acid-03.png" alt="Acid" height="256px">

[![Contribute!](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/Equilibrium-Games/Acid/issues)

Acid is a high speed, modular, light game framework written in C++17 providing multiple features such as cross-platform support.

The Vulkan API is used as the only rendering API; therefore, Vulkan structs and commands can be used directly in games. Acid allows for direct usage of Vulkan API calls but provides more than enough function to where this type of usage can be avoided.

Features are broken down into modules, modules can be added or removed from Acid with ease, and modules are easy to create. Vulkan is the only supported rendering API; however, APIs like Molten are supported through libraries, eventually, OpenGL and DirectX will be supported in this way.

Acid is licensed under MIT, we are open to contributions use the 'Developer Setup' section in the README to get started with Acid, and if you want to know about our code style read GUIDELINES.

## Builds
Windows   [![Build status](https://ci.appveyor.com/api/projects/status/e1an80wt6rb5nfk3?svg=true)](https://ci.appveyor.com/project/Mattparks/acid)

Linux   [![Build Status](https://travis-ci.org/Equilibrium-Games/Acid.svg?branch=master)](https://travis-ci.org/Equilibrium-Games/Acid)

# Code snippets
```cpp
// Imports a 2D texture.
auto guiBlack = Texture::Resource("Guis/Black.png");

// Imports a 3D cubemap.
auto skyboxSnowy = Cubemap::Resource("Objects/SkyboxSnowy", ".png");

// Imports a model.
auto dragon = ModelObj::Resource("Objects/Testing/ModelDragon.obj");

// Plays a 3D sound, at the origin, at half volume.
auto jump = Sound("Sounds/Jump.ogg", 0.5f);
jump.SetPosition(Vector3::ZERO);
jump.Play();

// Imports a game object.
auto playerObject = new GameObject("Objects/Player/Player.json", Transform());

// Creates a game object.
auto sphere = new GameObject(Transform(Vector3(6.7f, 6.7f, -8.0f), Vector3::ZERO, 3.0f));
sphere->AddComponent<Mesh>(ShapeSphere::Resource(30, 30, 1.0f));
sphere->AddComponent<ShapeSphere>(2.0f);
sphere->AddComponent<Rigidbody>(2.0f);
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
Trello   [Board](https://trello.com/b/ZRvpbbYC)

Readme   [Read more](https://equilibrium-games.github.io/Acid/)

Documentation   [Autogen docs](https://equilibrium-games.github.io/Acid-Docs/)

Metrics   [Code metrics](https://equilibrium-games.github.io/Acid-Docs/loc/LocMetrics.html)

Website   [Learn more](https://equilibrium.games)

## Platforms
Acid currently can be run on the following platforms:
 * Windows, Linux, MacOS

## Features
This is a list of current features in Acid:
 * On the fly GLSL to SPIR-V compiler
 * Modular rendering and compute pipeline
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
 * Animations loading (COLLADA)
 * Image file loading (JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC)
 * Config and class loading/saving (CSV, JSON, XML)
 * GameObject prefab loading/saving (JSON, XML)
 * Visual drivers and maths classes
 * Flexible input classes
 * C# generated files
 * Networking (WIP)
 * Resource packaging (WIP)
 * Steam integration (WIP)

## Dependencies
Acid uses the following libraries:
 * [Vulkan](https://www.khronos.org/vulkan) - Vulkan interface
 * [Glslang](https://github.com/KhronosGroup/glslang) - Shader compiling
 * [GLFW](https://github.com/glfw/glfw) - Window creation
 * [OpenAL](http://kcat.strangesoft.net/openal.html) - Audio interface
 * [Bullet3](https://github.com/bulletphysics/bullet3) - Physics integration
 * [STB](https://github.com/nothings/stb) - Texture and OGG loading

## Screenshots
<img src="https://raw.githubusercontent.com/Equilibrium-Games/Acid/master/Documents/Screenshot1.png" alt="Acid" width="600px">

<img src="https://raw.githubusercontent.com/Equilibrium-Games/Acid/master/Documents/Screenshot2.png" alt="Acid" width="600px">

<img src="https://raw.githubusercontent.com/Equilibrium-Games/Acid/master/Documents/Screenshot3.png" alt="Acid" width="600px">

<img src="https://raw.githubusercontent.com/Equilibrium-Games/Acid/master/Documents/Screenshot4.png" alt="Acid" width="600px">

## License
Acid is released under the MIT licence, [LICENSE.md](LICENSE.md) for more details.

## Developer Setup
Once Acid is cloned run `git submodule update --init --recursive` to update the submodules needed to compile. All platforms depend on CMake to generate IDE/make files.

The [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) and [OpenAL SDK](https://www.openal.org/downloads/) is required to develop and run Acid.

On Windows, Acid requires Visual Studios 2015 or later and the "Windows 10 SDK (10.0.15063.0) for UWP: C++". Use the VS installer and ensure "Desktop development with C++" and the Windows SDK is installed and up to date. Install the latest [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) and Acid should compile properly. Run Scripts/generate_vs.bat and an environment will be setup in Build.

Ensure you are using a compiler with full c++17 support, on Windows we recommend using MSVC or [MinGW w64](https://sourceforge.net/projects/mingw-w64/?source=navbar).

On Linux a GCC/Clang compiler is required, each package listed here is for Debian/Apt. Acid requires `xorg-dev`, `libopenal1`, and `libvulkan1` to be available. Read about how to setup [Vulkan on Linux](https://vulkan.lunarg.com/doc/sdk/latest/linux/getting_started.html).

Setup on MacOS is similar to the setup on Linux, except MoltenVK is used instead of Vulkan.

## Resources
Once CMake has loaded, link the Resources folder into the output directory using `Scripts/link_resources.bat` or `.sh`.

Old resources have been removed from the main repo, resources for commits from before April 4 2018 can be found on this fork: [https://github.com/mattparks/Flounder](https://github.com/mattparks/Folder).

## Contributing
You can contribute to Acid in any way you want, we are always looking for help.
