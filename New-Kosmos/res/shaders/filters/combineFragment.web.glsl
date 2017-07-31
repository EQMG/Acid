precision highp float;

//---------IN------------
varying highp vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalTexture;
layout(binding = 1) uniform sampler2D otherTexture;
uniform highp vec4 slideSpace;

//---------MAIN------------
void main(void)
{
   if (slideSpace.x <= pass_textureCoords.x && slideSpace.y >= pass_textureCoords.x && slideSpace.z <= pass_textureCoords.y && slideSpace.w >= pass_textureCoords.y) 
   {
	   gl_FragColor = texture2D(originalTexture, pass_textureCoords);
   } else {
	   gl_FragColor = texture2D(otherTexture, pass_textureCoords);
   }
}
