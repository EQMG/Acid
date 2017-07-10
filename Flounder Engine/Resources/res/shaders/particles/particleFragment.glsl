#version

//---------IN------------
in vec2 textureCoords1;
in vec2 textureCoords2;
in float textureBlendFactor;
in float textureTransparency;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D particleTexture;

//---------OUT------------
layout(location = 0) out vec4 out_albedo;
layout(location = 1) out vec4 out_normals;
layout(location = 2) out vec4 out_extras;

//---------MAIN------------
void main(void) {
	vec4 colour1 = texture(particleTexture, textureCoords1);
	vec4 colour2 = texture(particleTexture, textureCoords2);

	out_albedo = mix(colour1, colour2, textureBlendFactor);
	out_albedo.a -= textureTransparency;

	if (out_albedo.a <= 0.2) {
		out_albedo = vec4(0.0);
	    out_normals = vec4(0.0);
	    out_extras = vec4(0.0);
		discard;
	}

	out_normals = vec4(0.0, 1.0, 0.0, 1.0);
	out_extras = vec4(1.0, 0.0, 2.0 / 3.0, 1.0);
}
