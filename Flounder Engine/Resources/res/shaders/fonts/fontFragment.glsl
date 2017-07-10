#version

//---------INCLUDES------------
#include "maths.glsl"

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D fontTexture;
uniform bool polygonMode;
uniform vec4 colour;
uniform vec3 borderColour;
uniform vec2 borderSizes;
uniform vec2 edgeData;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------MAIN------------
void main(void) {
	float distance = texture(fontTexture, pass_textureCoords).a;
	float alpha = fsmoothlyStep((1.0 - edgeData.x) - edgeData.y, 1.0 - edgeData.x, distance);
	float outlineAlpha = fsmoothlyStep((1.0 - borderSizes.x) - borderSizes.y, 1.0 - borderSizes.x, distance);
	float overallAlpha = alpha + (1.0 - alpha) * outlineAlpha;
	vec3 overallColour = mix(borderColour, colour.rgb, alpha / overallAlpha);

	out_colour = vec4(overallColour, overallAlpha);
	out_colour.a *= colour.a;

	if (polygonMode) {
		out_colour = vec4(1.0, 0.0, 0.0, colour.a);
	}

	if (out_colour.a < 0.05){
		out_colour = vec4(0.0);
		discard;
	}
}
