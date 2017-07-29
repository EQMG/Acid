precision highp float;

//---------INCLUDES------------
#include "res/shaders/maths.glsl"

//---------IN------------
varying highp vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D fontTexture;
uniform bool polygonMode;
uniform highp vec4 colour;
uniform highp vec3 borderColour;
uniform highp vec2 borderSizes;
uniform highp vec2 edgeData;

//---------MAIN------------
void main(void) 
{
	float distance = texture2D(fontTexture, pass_textureCoords).a;
	float alpha = fsmoothlyStep((1.0 - edgeData.x) - edgeData.y, 1.0 - edgeData.x, distance);
	float outlineAlpha = fsmoothlyStep((1.0 - borderSizes.x) - borderSizes.y, 1.0 - borderSizes.x, distance);
	float overallAlpha = alpha + (1.0 - alpha) * outlineAlpha;
	vec3 overallColour = mix(borderColour, colour.rgb, alpha / overallAlpha);

	gl_FragColor = vec4(overallColour, overallAlpha);
	gl_FragColor.a *= colour.a;

	if (polygonMode) 
	{
		gl_FragColor = vec4(1.0, 0.0, 0.0, colour.a);
	}

	if (gl_FragColor.a < 0.05)
	{
		gl_FragColor = vec4(0.0);
		discard;
	}
}
