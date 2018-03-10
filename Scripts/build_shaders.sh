cd ../

find Resources/Shaders -type f -iname "*.frag" -exec Libraries/glslangValidator -V {} -o {}.spv \;
find Resources/Shaders -type f -iname "*.vert" -exec Libraries/glslangValidator -V {} -o {}.spv \;
find Resources/Shaders -type f -iname "*.geom" -exec Libraries/glslangValidator -V {} -o {}.spv \;
find Resources/Shaders -type f -iname "*.comp" -exec Libraries/glslangValidator -V {} -o {}.spv \;
