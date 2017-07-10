#version

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalTexture;
uniform float blurAmount;
uniform float centre;
uniform float stepSize;
uniform float steps;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------MAIN------------
void main(void) {
	// Work out how much to blur based on the mid point.
	float amount = pow((pass_textureCoords.y * centre) * 2.0 - 1.0, 2.0) * blurAmount;
	float offsetMin = (float(steps - 1.0)) / -2.0;
    float offsetMax = (float(steps - 1.0)) / +2.0;
		
	// This is the accumulation of color from the surrounding pixels in the texture.
	out_colour = vec4(0.0, 0.0, 0.0, 1.0);

	// From minimum offset to maximum offset.
	for (float offsetX = offsetMin; offsetX <= offsetMax; ++offsetX) {
		for (float offsetY = offsetMin; offsetY <= offsetMax; ++offsetY) {
			// Copy the coord so we can mess with it.
			vec2 tempTextureCoords = pass_textureCoords.xy;

			// Work out which uv we want to sample now.
			tempTextureCoords.x += offsetX * amount * stepSize;
			tempTextureCoords.y += offsetY * amount * stepSize;

			// Accumulate the sample
			out_colour += texture(originalTexture, tempTextureCoords);
		}
	}
		
	// Because we are doing an average, we divide by the amount (x AND y, hence STEPS * steps).
	out_colour /= float(steps * steps);
}
