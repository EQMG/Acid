<img src="https://raw.githubusercontent.com/Equilibrium-Games/Acid/master/Documents/Acid_03.png" alt="Acid" height="130px">

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.me/mattparks5855)
[![Build status](https://ci.appveyor.com/api/projects/status/e1an80wt6rb5nfk3?svg=true)](https://ci.appveyor.com/project/Mattparks/acid)
[![Build Status](https://travis-ci.org/Equilibrium-Games/Acid.svg?branch=master)](https://travis-ci.org/Equilibrium-Games/Acid)

Acid is a high speed, cross-platform, game engine written in C++17. Features are broken down into modules, modules can be added or removed from Acid with ease.

Vulkan is used as the only rendering API, as a result Vulkan can be used directly in games, but should be avoided. Metal is supported through [MoltenVK](https://github.com/KhronosGroup/MoltenVK), eventually OpenGL and DirectX will be supported in a similar way.

Acid is licensed under MIT and is open to contributions, use the 'Compiling' section in the [README.md](README.md) to get started with Acid.

## Links
Readme   [Read more](https://equilibrium-games.github.io/Acid/)

Documentation   [Autogen docs](https://equilibrium-games.github.io/Acid-Docs/)

Website   [Learn more](https://equilibrium.games)

Trello   [Board](https://trello.com/b/ZRvpbbYC)

## Features
This is a list of current features in Acid:
 * Multiplatform (32bit and 64bit)
 * Multithreaded command buffers and thread safety
 * On the fly GLSL to SPIR-V compilation and reflection
 * Modular rendering pipeline and compute
 * Deferred rendering (PBR, Simple)
 * Bullet physics
 * Networking (HTTP, FTP, UDP, TCP)
 * Frustum culling
 * Resource management
 * Event listener system
 * File path searches, and packaging
 * GUI and SDF font rendering
 * Entity component system
 * Particle effect systems
 * Audio and music
 * Skyboxes and fog (OGG, WAV)
 * Shadow mapping
 * Post effects (Lensflare, Glow, Blur, SSAO, ...)
 * Model file loading (OBJ)
 * Animations loading (COLLADA)
 * Image file loading (JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC)
 * Entity prefab loading/saving (JSON, XML)
 * Time, vectors, and matrices
 * Flexible input classes
 * Steam integration (WIP)
 * C# generated files (https://github.com/Equilibrium-Games/Acid-Sharp)
 * Scene editor (https://github.com/Equilibrium-Games/Acid-Editor)

## Dependencies
Acid uses the following libraries:
 * [Vulkan](https://www.khronos.org/vulkan) - Vulkan interface
 * [Glslang](https://github.com/KhronosGroup/glslang) - Shader compiling
 * [GLFW](https://github.com/glfw/glfw) - Window creation
 * [OpenAL](http://kcat.strangesoft.net/openal.html) - Audio interface
 * [Bullet3](https://github.com/bulletphysics/bullet3) - Physics integration
 * [PhysFS](https://icculus.org/physfs) - Archive file access
 * [STB](https://github.com/nothings/stb) - Texture and OGG loading
 * [FastNoise](https://github.com/Auburns/FastNoise) - Noise generation
 * [Freetype2](https://www.freetype.org/) - Font handling in text objects

# Code Snippets
```cpp
// Imports a 2D texture using nearest filtering.
auto guiBlack = Texture::Create("Guis/Black.png", VK_FILTER_NEAREST);

// Imports a 3D cubemap (face names defined in Cubemap.cpp).
auto skyboxSnowy = Cubemap::Create("Objects/SkyboxSnowy", ".png");

// Imports a OBJ model.
auto dragon = ModelObj::Create("Objects/Testing/ModelDragon.obj");

// Creates a sphere model with 20 latitude and longitude bands with a radius of 1.
auto sphere = ModelSphere::Create(20, 20, 1.0f);

// Plays a 3D sound (sound buffer resource internally managed), at (10, 0, 0), at half volume.
auto jump = Sound("Sounds/Jump.ogg", Transform(10.0f * Vector3::Right), Audio::Type::Effect, false, true, 0.5f);

// Loads a entity from a prefab file.
auto playerObject = GetStructure()->CreateEntity("Objects/Player/Player.json", Transform::Identity);

// Creates a entity.
auto sphere = GetStructure()->CreateEntity(Transform(Vector3(6.7f, 6.7f, -8.0f), Vector3::ZERO, 3.0f));
sphere->AddComponent<Mesh>(ShapeSphere::Create(20, 20, 1.0f)); // This will used the sphere buffers created earlier.
sphere->AddComponent<ShapeSphere>(); // Multiple shape components can be added to a single rigidbody.
sphere->AddComponent<Rigidbody>(2.0f); // Will be created weighing 2 units, this will find all shapes attached.
sphere->AddComponent<MaterialDefault>(Colour::White, Texture::Create("Objects/Testing/Albedo.png"), 0.0f, 0.5f,
    Texture::Create("Objects/Testing/Material.png"), Texture::Create("Objects/Testing/Normal.png"));
sphere->AddComponent<MeshRender>(); // A mesh renderer will render the material attached.

// Vector maths.
Vector2 a(3.0f, -7.2f);
Vector2 b(-1.74f, 15.4f);
Vector2 c = a * b;
float dist = a.Distance(b);

// Split a string by spaces.
std::string stringSource = "Hello world!";
std::vector<std::string> stringSplit = String::Split(stringSource, " ");

// Will run a lambda after 5 seconds.
Events::Get()->AddEvent<EventTime>([](){
	Log::Out("Hello world: %f\n", Maths::Random(-1.0f, 1.0f));
}, Time::Seconds(5.0f));
```

## Screenshots
<img src="https://raw.githubusercontent.com/Equilibrium-Games/Acid/master/Documents/Screenshot1.png" alt="Acid" width="600px">

<img src="https://raw.githubusercontent.com/Equilibrium-Games/Acid/master/Documents/Screenshot2.png" alt="Acid" width="600px">

<img src="https://raw.githubusercontent.com/Equilibrium-Games/Acid/master/Documents/Screenshot3.png" alt="Acid" width="600px">

<img src="https://raw.githubusercontent.com/Equilibrium-Games/Acid/master/Documents/Screenshot4.png" alt="Acid" width="600px">

<img src="https://raw.githubusercontent.com/Equilibrium-Games/Acid/master/Documents/Screenshot5.png" alt="Acid" width="600px">

## Compiling
If you don't want to use system-wide libraries, then after cloning the repo, run `git submodule update --init --recursive` in the Acid directory to update the submodules.

All platforms depend on [CMake](https://cmake.org/download) to generate IDE/make files.

Cmake options (default ON):
* `BUILD_TESTS`
* `ACID_INSTALL_EXAMPLES`
* `ACID_INSTALL_RESOURCES`  

If you installed Acid using only system libs, then `find_package(Acid)` will work from Cmake. Versioning is also supported.  
When using `find_package(Acid)` the imported target `Acid::Acid` will be created.  
The `ACID_RESOURCES_DIR` variable will also be available, which will point to the on-disk location of `Acid/Resources` (if installed).

[Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) and [OpenAL SDK](https://openal-soft.org/#download) are required to develop and run Acid.

Make sure you have environment variables `VULKAN_SDK` and `OPENALDIR` set to the paths you have Vulkan and OpenAL installed into.

Ensure you are using a compiler with full C++17 support, on Windows it is recommend you use MSVC or [MinGW w64](https://sourceforge.net/projects/mingw-w64/?source=navbar).

If using Visual Studio it must be 2015 or later. Use the Visual Studio installer and select both "Desktop development with C++" and "Windows SDK" if they are not already installed. Then on Visual Studio Acid can be opened as a CMake workspace folder.

On Linux Acid requires `xorg-dev`, `libopenal1`, and `libvulkan1` to be installed. Read about how to setup [Vulkan on Linux](https://vulkan.lunarg.com/doc/sdk/latest/linux/getting_started.html) so a Vulkan SDK is found.

Setup on MacOS is similar to the setup on Linux, a compiler that supports C++17 is required, such as XCode 10.0.

## Contributing
You can contribute to Acid in any way you want, we are always looking for help. You can learn about Acid's code style from the [Documents/GUIDELINES.md](Documents/GUIDELINES.md).
