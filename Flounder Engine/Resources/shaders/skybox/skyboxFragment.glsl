#version

//---------INCLUDES------------
#include "maths.glsl"

//---------IN------------
in vec3 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform samplerCube cubeMap;
uniform bool polygonMode;
uniform vec3 skyColour;
uniform float blendFactor;

//---------OUT------------
layout(location = 0) out vec4 out_albedo;
layout(location = 1) out vec4 out_normals;
layout(location = 2) out vec4 out_extras;

//---------MAIN------------
void main(void) {
    vec3 cubemapColour = texture(cubeMap, pass_textureCoords).rgb;

	out_albedo = vec4(skyColour + mix(vec3(0.0), cubemapColour, blendFactor), 1.0);

	if (polygonMode) {
		out_albedo = vec4(1.0, 0.0, 0.0, 1.0);
	}

	out_normals = vec4(vec3(0.0, 1.0, 0.0) + 1.0 / 2.0, 0.0);
	out_extras = vec4(1.0, 0.0, 1.0, 1.0); // Ignores lighting.
}