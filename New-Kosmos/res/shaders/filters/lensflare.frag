#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0) uniform sampler2D albedoSampler;
layout(binding = 2) uniform sampler2D extrasSampler;

layout (binding = 3) uniform UBO
{
	vec3 sunPosition;
	float worldHeight;
	vec2 displaySize;
} ubo;

layout(location = 0) in vec2 textureCoords;

layout(location = 0) out vec4 outColour;

bool insideScreen(vec2 test) 
{
    return test.x >= 0.0 && test.x <= 1.0 && test.y >= 0.0 && test.y <= 1.0;
}

void main(void) 
{
    vec2 sunCoord = (ubo.sunPosition.xy + 1.0) / 2.0;

	vec4 albedo = texture(albedoSampler, textureCoords);
	float glow = texture(extrasSampler, sunCoord).g;
	bool process = ubo.sunPosition.z >= 0.0 && (glow > 0.4 || !insideScreen(sunCoord));
	vec2 uv = (textureCoords - 0.5) * (ubo.displaySize.x / ubo.displaySize.y);

	vec2 uvd = uv * length(uv);
	vec3 colour = vec3(0.0);

    if (process) 
	{
        vec2 uvx = mix(uv, uvd, 1.0);
        float f2 = max(1.0 / (1.0 + 32.0 * pow(length(uvx + 0.8 * ubo.sunPosition.xy), 2.0)), 0.0) * 0.25;
        float f22 = max(1.0 / (1.0 + 32.0 * pow(length(uvx + 0.85 * ubo.sunPosition.xy), 2.0)), 0.0) * 0.23;
        float f23 = max(1.0 / (1.0 + 32.0 * pow(length(uvx + 0.9 * ubo.sunPosition.xy), 2.0)), 0.0) * 0.21;

        uvx = mix(uv, uvd, -0.5);
        float f4 = max(0.01 - pow(length(uvx + 0.4 * ubo.sunPosition.xy), 2.4), 0.0) * 6.0;
        float f42 = max(0.01 - pow(length(uvx + 0.45 * ubo.sunPosition.xy), 2.4), 0.0) * 5.0;
        float f43 = max(0.01 - pow(length(uvx + 0.5 * ubo.sunPosition.xy), 2.4), 0.0) * 3.0;

        uvx = mix(uv, uvd, -0.4);
        float f5 = max(0.01 - pow(length(uvx + 0.2 * ubo.sunPosition.xy), 5.5), 0.0) * 2.0;
        float f52 = max(0.01 - pow(length(uvx + 0.4 * ubo.sunPosition.xy), 5.5), 0.0) * 2.0;
        float f53 = max(0.01 - pow(length(uvx + 0.6 * ubo.sunPosition.xy), 5.5), 0.0) * 2.0;

        uvx = mix(uv, uvd, -0.5);
        float f6 = max(0.01 - pow(length(uvx - 0.3 * ubo.sunPosition.xy), 1.6), 0.0) * 6.0;
        float f62 = max(0.01 - pow(length(uvx - 0.325 * ubo.sunPosition.xy), 1.6), 0.0) * 3.0;
        float f63 = max(0.01 - pow(length(uvx - 0.35 * ubo.sunPosition.xy), 1.6), 0.0) * 5.0;

        colour.r += f2 + f4 + f5 + f6;
        colour.g += f22 + f42 + f52 + f62;
        colour.b += f23 + f43 + f53 + f63;
	}

    // Hides flare when below a world height.
    colour *= clamp(ubo.worldHeight + 10.0, 0.0, 1.0);

    // Adds a bit of darkining around the edge of the screen.
	colour = colour * 1.3 - vec3(length(uvd) * 0.05);

	outColour = albedo + vec4(colour, 0.0);
}
