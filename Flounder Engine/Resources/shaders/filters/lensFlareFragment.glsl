#version

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalAlbedo;
layout(binding = 2) uniform sampler2D originalExtras;
uniform vec3 sunPosition;
uniform float worldHeight;
uniform vec2 displaySize;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------LENS FLARE------------
vec3 lensflare(bool process, vec2 uv, vec3 pos) {
	vec2 uvd = uv * length(uv);
	vec3 colour = vec3(0.0);

    if (process) {
        vec2 uvx = mix(uv, uvd, 1.0);
        float f2 = max(1.0 / (1.0 + 32.0 * pow(length(uvx + 0.8 * pos.xy), 2.0)), 0.0) * 0.25;
        float f22 = max(1.0 / (1.0 + 32.0 * pow(length(uvx + 0.85 * pos.xy), 2.0)), 0.0) * 0.23;
        float f23 = max(1.0 / (1.0 + 32.0 * pow(length(uvx + 0.9 * pos.xy), 2.0)), 0.0) * 0.21;

        uvx = mix(uv, uvd, -0.5);
        float f4 = max(0.01 - pow(length(uvx + 0.4 * pos.xy), 2.4), 0.0) * 6.0;
        float f42 = max(0.01 - pow(length(uvx + 0.45 * pos.xy), 2.4), 0.0) * 5.0;
        float f43 = max(0.01 - pow(length(uvx + 0.5 * pos.xy), 2.4), 0.0) * 3.0;

        uvx = mix(uv, uvd, -0.4);
        float f5 = max(0.01 - pow(length(uvx + 0.2 * pos.xy), 5.5), 0.0) * 2.0;
        float f52 = max(0.01 - pow(length(uvx + 0.4 * pos.xy), 5.5), 0.0) * 2.0;
        float f53 = max(0.01 - pow(length(uvx + 0.6 * pos.xy), 5.5), 0.0) * 2.0;

        uvx = mix(uv, uvd, -0.5);
        float f6 = max(0.01 - pow(length(uvx - 0.3 * pos.xy), 1.6), 0.0) * 6.0;
        float f62 = max(0.01 - pow(length(uvx - 0.325 * pos.xy), 1.6), 0.0) * 3.0;
        float f63 = max(0.01 - pow(length(uvx - 0.35 * pos.xy), 1.6), 0.0) * 5.0;

        colour.r += f2 + f4 + f5 + f6;
        colour.g += f22 + f42 + f52 + f62;
        colour.b += f23 + f43 + f53 + f63;
	}

    // Hides flare when below a world height.
    colour *= clamp(worldHeight + 10.0, 0.0, 1.0);

    // Adds a bit of darkining around the edge of the screen.
	return colour * 1.3 - vec3(length(uvd) * 0.05);
}

//---------INSIDE SCREEN------------
bool insideScreen(vec2 test) {
    return test.x >= 0.0 && test.x <= 1.0 && test.y >= 0.0 && test.y <= 1.0;
}

//---------MAIN------------
void main(void) {
    vec2 sunCoord = (sunPosition.xy + 1.0) / 2.0;

	vec4 albedo = texture(originalAlbedo, pass_textureCoords);
	float glow = texture(originalExtras, sunCoord).g;
	bool process = sunPosition.z >= 0.0 && (glow > 0.4 || !insideScreen(sunCoord));

	vec3 colour = vec3(1.4, 1.2, 1.0) * lensflare(process, (pass_textureCoords - 0.5) * (displaySize.x / displaySize.y), sunPosition);
	out_colour = albedo + vec4(colour, 0.0);
}
