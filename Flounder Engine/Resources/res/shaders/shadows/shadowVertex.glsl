#version

//---------CONSTANT------------
const int MAX_JOINTS = 50;
const int MAX_WEIGHTS = 3;

//---------IN------------
layout(binding = 1) uniform sampler2D swayMap;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_textureCoords;
layout(location = 4) in ivec3 in_jointIndices;
layout(location = 5) in vec3 in_weights;

//---------UNIFORM------------
uniform mat4 mvpMatrix;

uniform bool animated;
uniform mat4 jointTransforms[MAX_JOINTS];

uniform bool swaying;
uniform float swayHeight;
uniform vec2 swayOffset;

//---------MAIN------------
void main(void) {
	vec4 totalLocalPos = vec4(0.0);

    if (animated) {
        for (int i = 0; i < MAX_WEIGHTS; i++){
		    mat4 jointTransform = jointTransforms[in_jointIndices[i]];
            vec4 posePosition = jointTransform * vec4(in_position, 1.0);
            totalLocalPos += posePosition * in_weights[i];
        }
	} else {
	    totalLocalPos = vec4(in_position, 1.0);
	}

	if (swaying) {
	    vec4 swayColour = texture(swayMap, in_textureCoords);
	    float swayPower = 0.5 * exp(log(length(swayColour.rgb)) / 3.0) * (totalLocalPos.y / swayHeight) * length(totalLocalPos.xyz);
	    totalLocalPos.x += swayPower * swayOffset.x;
	    totalLocalPos.z += swayPower * swayOffset.y;
	}

	gl_Position = mvpMatrix * totalLocalPos;
}