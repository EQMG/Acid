#version

//---------CONSTANT------------
const int MAX_JOINTS = 50;
const int MAX_WEIGHTS = 3;

//---------IN------------
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_textureCoords;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec3 in_tangent;
layout(location = 4) in ivec3 in_jointIndices;
layout(location = 5) in vec3 in_weights;

//---------UNIFORM------------
layout(binding = 2) uniform sampler2D swayMap;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec4 clipPlane;
uniform mat4 modelMatrix;

uniform float atlasRows;
uniform vec2 atlasOffset;

uniform bool animated;
uniform mat4 jointTransforms[MAX_JOINTS];

uniform bool swaying;
uniform float swayHeight;
uniform vec2 swayOffset;

//---------OUT------------
out vec2 pass_textureCoords;
out vec3 pass_surfaceNormal;

//---------MAIN------------
void main(void) {
	vec4 totalLocalPos = vec4(0.0);
	vec4 totalNormal = vec4(0.0);

    if (animated) {
        for (int i = 0; i < MAX_WEIGHTS; i++){
		    mat4 jointTransform = jointTransforms[in_jointIndices[i]];
            vec4 posePosition = jointTransform * vec4(in_position, 1.0);
            totalLocalPos += posePosition * in_weights[i];

            vec4 worldNormal = jointTransform * vec4(in_normal, 0.0);
            totalNormal += worldNormal * in_weights[i];
        }
	} else {
	    totalLocalPos = vec4(in_position, 1.0);
	    totalNormal = vec4(in_normal, 0.0);
	}

	pass_textureCoords = (in_textureCoords / atlasRows) + atlasOffset;

	if (swaying) {
	    vec4 swayColour = texture(swayMap, in_textureCoords);
	    float swayPower = 0.5 * exp(log(length(swayColour.rgb)) / 3.0) * (totalLocalPos.y / swayHeight) * length(totalLocalPos.xyz);
	    totalLocalPos.x += swayPower * swayOffset.x;
	    totalLocalPos.z += swayPower * swayOffset.y;
	}

	vec4 worldPosition = modelMatrix * totalLocalPos;

	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	gl_Position = projectionMatrix * viewMatrix * worldPosition;

	pass_surfaceNormal = normalize((modelMatrix * totalNormal).xyz);
}
