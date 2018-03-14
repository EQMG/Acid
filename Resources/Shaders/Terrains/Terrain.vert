#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
} scene;

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;

	float worldCurvature;
} object;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUv;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;

layout(location = 0) out vec3 fragmentNormal;
layout(location = 1) out vec2 fragmentUv;
layout(location = 2) out vec3 fragmentColour;

out gl_PerVertex 
{
	vec4 gl_Position;
};

vec4 computeWorld(vec4 pos)
{
    const float planetRadius = object.worldCurvature;

    if (planetRadius == 0.0f)
    {
        return pos;
    }

    const vec3 camPos = -transpose(mat3(scene.view)) * scene.view[3].xyz;

	// Complex approach.
	vec2 planedir = normalize(vec2(pos.x - camPos.x, pos.z - camPos.z));
	vec2 plane = vec2(pos.y - camPos.y, sqrt((pos.x - camPos.x) * (pos.x - camPos.x) + (pos.z - camPos.z) * (pos.z - camPos.z)));
	vec2 zexp = plane / planetRadius;
	zexp = exp(zexp.x) * vec2(cos(zexp.y), sin(zexp.y));
	vec2 circle = planetRadius * zexp - vec2(planetRadius, 0.0f);

	vec4 world = vec4(pos);
	world.x = circle.y * planedir.x + camPos.x;
	world.z = circle.y * planedir.y + camPos.z;
	world.y = circle.x + camPos.y;
	return world;
}

void main() 
{
	vec4 totalLocalPos = vec4(vertexPosition, 1.0f);
	vec4 totalNormal = vec4(vertexNormal, 0.0f);

	vec4 worldPosition = computeWorld(object.transform * totalLocalPos);

	gl_Position = scene.projection * scene.view * worldPosition;

	fragmentNormal = normalize((object.transform * totalNormal).xyz);
	fragmentUv = vertexUv;
	fragmentColour = vertexTangent;
}
