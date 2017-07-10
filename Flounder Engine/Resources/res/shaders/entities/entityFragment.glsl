#version

//---------IN------------
in vec2 pass_textureCoords;
in vec3 pass_surfaceNormal;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform sampler2D glowMap;

//---------OUT------------
layout(location = 0) out vec4 out_albedo;
layout(location = 1) out vec4 out_normals;
layout(location = 2) out vec4 out_extras;

uniform vec3 colourOffset;
uniform vec3 colourAddition;

uniform float transparency;

uniform float shineDamper;
uniform float reflectivity;

uniform bool ignoreFog;
uniform bool ignoreLighting;

uniform bool useGlowMap;

//---------MAIN------------
void main(void) {
	vec4 diffuseColour = texture(diffuseMap, pass_textureCoords);

	if (diffuseColour.a < 0.1){
		out_albedo = vec4(0.0);
	    out_normals = vec4(0.0);
	    out_extras = vec4(0.0);
		discard;
	}

	bool glowing = false;
	float glow = reflectivity;

	if (useGlowMap) {
	    vec4 glowColour = texture(glowMap, pass_textureCoords);
	    glow += length(glowColour.rgb);

	    if (glowColour.r > 0.5) {
	        glowing = true;
	    }
	}

	out_albedo = vec4(diffuseColour + vec4(colourOffset, 0.0)) + vec4(colourAddition, 0.0);
	out_albedo.a *= 1.0 - transparency;
	out_normals = vec4(pass_surfaceNormal + 1.0 / 2.0, out_albedo.a);
	out_extras = vec4(shineDamper, glow, (1.0 / 3.0) * (float(ignoreFog) + (2.0 * float(ignoreLighting || glowing))), out_albedo.a);
}