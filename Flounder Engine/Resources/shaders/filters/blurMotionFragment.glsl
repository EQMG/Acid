#version

//---------INCLUDES------------
#include "maths.glsl"

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalTexture;
layout(binding = 1) uniform sampler2D originalDepth;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 lastViewMatrix;
uniform float delta;

const float numSamples = 8.0;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------CALCULATE LOCATION------------
vec3 decodeLocation() {
    float depth = texture(originalDepth, pass_textureCoords).r;
    vec4 p = finverse(projectionMatrix) * (vec4(pass_textureCoords, depth, 1.0) * 2.0 - 1.0);
    return vec3(finverse(viewMatrix) * vec4(p.xyz / p.w, 1.0));
}

//---------MAIN------------
void main(void) {
    // Gets the currebt world position for this fragment.
	vec4 worldPosition = vec4(decodeLocation(), 1.0);

    // Transform by the view-projection inverse to get the current world position.
    vec4 currentPos = projectionMatrix * viewMatrix * worldPosition;

    // Use the world position, and transform by the previous view-projection matrix.
    vec4 previousPos = projectionMatrix * lastViewMatrix * worldPosition;

    // Use this frame's position and last frame's to compute the pixel velocity.
    vec2 velocity = (currentPos.xy - previousPos.xy) * 0.02 * delta;

    // Samples the texture to produce a blur in the velocity.
    vec3 sampled_colour = vec3(0.0);

    for (float i = 1.0; i < numSamples; ++i){
        sampled_colour += texture(originalTexture, pass_textureCoords + (i * velocity)).rgb;
    }

    out_colour = vec4(sampled_colour / numSamples, 1.0);
}
