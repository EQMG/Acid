#version

//---------IN------------
flat in vec3 pass_surfaceNormal;

//---------UNIFORM------------

//---------OUT------------
layout(location = 0) out vec4 out_albedo;
layout(location = 1) out vec4 out_normals;
layout(location = 2) out vec4 out_extras;

uniform float shineDamper;
uniform float reflectivity;

//---------MAIN------------
void main(void) {
	vec4 diffuseColour = vec4(0.0, 1.0, 0.0, 1.0);

	out_albedo = diffuseColour;
	out_normals = vec4(pass_surfaceNormal + 1.0 / 2.0, out_albedo.a);
	out_extras = vec4(shineDamper, reflectivity, 0.0, out_albedo.a);
}