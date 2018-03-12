cd ../

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
find ${SCRIPTPATH}/Resources/Shaders -type f -iname "*.frag" -exec ${SCRIPTPATH}/Libraries/glslangValidator -V {} -o {}.spv \;
find ${SCRIPTPATH}/Resources/Shaders -type f -iname "*.vert" -exec ${SCRIPTPATH}/Libraries/glslangValidator -V {} -o {}.spv \;
find ${SCRIPTPATH}/Resources/Shaders -type f -iname "*.geom" -exec ${SCRIPTPATH}/Libraries/glslangValidator -V {} -o {}.spv \;
find ${SCRIPTPATH}/Resources/Shaders -type f -iname "*.comp" -exec ${SCRIPTPATH}/Libraries/glslangValidator -V {} -o {}.spv \;
