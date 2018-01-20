#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboScene
{
	vec3 sunPosition;
	float worldHeight;
	vec2 displaySize;
} scene;

layout(set = 0, binding = 1) uniform sampler2D samplerColour;
layout(set = 0, binding = 2) uniform sampler2D samplerMaterial;

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeColour;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

bool insideScreen(vec2 test) 
{
	return test.x >= 0.0 && test.x <= 1.0 && test.y >= 0.0 && test.y <= 1.0;
}

void main() 
{
	vec2 sunCoord = (scene.sunPosition.xy + 1.0) / 2.0;

	vec4 textureColour = texture(samplerColour, fragmentUv);

	//float metallic = texture(samplerMaterial, sunCoord).r;
	//bool process = scene.sunPosition.z >= 0.0 && (metallic > 0.4 || !insideScreen(sunCoord));
	bool process = scene.sunPosition.z >= 0.0;

	vec2 uv = (fragmentUv - 0.5) * (scene.displaySize.x / scene.displaySize.y);

	vec2 uvd = uv * length(uv);
	vec3 colour = vec3(0.0);

	if (process) 
	{
		vec2 uvx = mix(uv, uvd, 1.0);
		float f2 = max(1.0 / (1.0 + 32.0 * pow(length(uvx + 0.8 * scene.sunPosition.xy), 2.0)), 0.0) * 0.25;
		float f22 = max(1.0 / (1.0 + 32.0 * pow(length(uvx + 0.85 * scene.sunPosition.xy), 2.0)), 0.0) * 0.23;
		float f23 = max(1.0 / (1.0 + 32.0 * pow(length(uvx + 0.9 * scene.sunPosition.xy), 2.0)), 0.0) * 0.21;

		uvx = mix(uv, uvd, -0.5);
		float f4 = max(0.01 - pow(length(uvx + 0.4 * scene.sunPosition.xy), 2.4), 0.0) * 6.0;
		float f42 = max(0.01 - pow(length(uvx + 0.45 * scene.sunPosition.xy), 2.4), 0.0) * 5.0;
		float f43 = max(0.01 - pow(length(uvx + 0.5 * scene.sunPosition.xy), 2.4), 0.0) * 3.0;

		uvx = mix(uv, uvd, -0.4);
		float f5 = max(0.01 - pow(length(uvx + 0.2 * scene.sunPosition.xy), 5.5), 0.0) * 2.0;
		float f52 = max(0.01 - pow(length(uvx + 0.4 * scene.sunPosition.xy), 5.5), 0.0) * 2.0;
		float f53 = max(0.01 - pow(length(uvx + 0.6 * scene.sunPosition.xy), 5.5), 0.0) * 2.0;

		uvx = mix(uv, uvd, -0.5);
		float f6 = max(0.01 - pow(length(uvx - 0.3 * scene.sunPosition.xy), 1.6), 0.0) * 6.0;
		float f62 = max(0.01 - pow(length(uvx - 0.325 * scene.sunPosition.xy), 1.6), 0.0) * 3.0;
		float f63 = max(0.01 - pow(length(uvx - 0.35 * scene.sunPosition.xy), 1.6), 0.0) * 5.0;

		colour.r += f2 + f4 + f5 + f6;
		colour.g += f22 + f42 + f52 + f62;
		colour.b += f23 + f43 + f53 + f63;
	}

	// Hides flare when below a world height.
	colour *= clamp(scene.worldHeight + 10.0, 0.0, 1.0);

	// Adds a bit of darkining around the edge of the screen.
	colour = colour * 1.3 - vec3(length(uvd) * 0.05);

	outColour = textureColour + vec4(colour, 0.0);

	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(fragmentUv * sizeColour), outColour);
}
