#version

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalTexture;
uniform vec3 screenColour;
uniform float curveAmountX;
uniform float curveAmountY;
uniform float scanLineSize;
uniform float scanIntensity;

uniform float moveTime;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------MAIN------------
void main(void) {
	vec2 tc = vec2(pass_textureCoords.x, pass_textureCoords.y);

	// Distance from the center
	float dx = abs(0.5 - tc.x);
	float dy = abs(0.5 - tc.y);

	// Square it to smooth the edges
	dx *= dx;
	dy *= dy;

	tc.x -= 0.5;
	tc.x *= 1.0 + (dy * curveAmountX);
	tc.x += 0.5;

	tc.y -= 0.5;
	tc.y *= 1.0 + (dx * curveAmountY);
	tc.y += 0.5;

	// Get texel, and add in scanline if need be
	vec4 colour = texture(originalTexture, vec2(tc.x, tc.y));
	colour.rgb += sin((tc.y + moveTime) * scanLineSize) * scanIntensity;

	// Cutoff
	if (tc.y > 1.0 || tc.x < 0.0 || tc.x > 1.0 || tc.y < 0.0) {
		colour = vec4(0.0);
	}

	float grey = dot(colour.xyz, vec3(0.299, 0.587, 0.114));
	out_colour = vec4(screenColour * grey, 1.0);
}
