#version 130

//---------IN------------
in vec3 pass_surfaceNormal;
in vec4 pass_clipSpace;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D reflectionMap;
uniform vec4 diffuseColour;

uniform float shineDamper;
uniform float reflectivity;

uniform bool ignoreReflections;

//---------OUT------------
layout(location = 0) out vec4 out_albedo;
layout(location = 1) out vec4 out_normals;
layout(location = 2) out vec4 out_extras;

//---------REFRACTION------------
vec2 getReflectionTexCoords(vec2 normalizedDeviceCoords){
	vec2 reflectionTextureCoords = vec2(normalizedDeviceCoords.x, -normalizedDeviceCoords.y);
	reflectionTextureCoords.x = clamp(reflectionTextureCoords.x, 0.001, 0.999);
	reflectionTextureCoords.y = clamp(reflectionTextureCoords.y, -0.999, -0.001);
	return reflectionTextureCoords;
}

//---------MAIN------------
void main(void) {
    if (!ignoreReflections) {
        vec2 normalizedDeviceCoords = (pass_clipSpace.xy / pass_clipSpace.w) / 2.0 + 0.5;
        vec2 reflectionTextureCoords = getReflectionTexCoords(normalizedDeviceCoords);
        vec3 reflectionColour = texture(reflectionMap, reflectionTextureCoords).rgb;
        out_albedo = vec4(mix(reflectionColour, diffuseColour.rgb, diffuseColour.a), 1.0);
	} else {
        out_albedo = vec4(diffuseColour.rgb, 1.0);
	}

	out_normals = vec4(pass_surfaceNormal + 1.0 / 2.0, 1.0);
	out_extras = vec4(shineDamper, reflectivity, (1.0 / 3.0) * (float(false) + 2.0 * float(false)), 1.0);
}