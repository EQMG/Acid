#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

struct GlyphInfo
{
    vec4 bbox;

    // point offset
    // cell offset
    // cell count in x
    // cell count in y
    uvec4 cellInfo;
};

layout(binding = 0) buffer GlyphBuffer
{
	GlyphInfo glyphs[];
} glyphBuffer;

layout(location = 0) in vec4 inRect;
layout(location = 1) in uint inGlyphIndex;
layout(location = 2) in float inSharpness;
layout(location = 3) in vec4 inColour;

layout(location = 0) out vec2 outGlyphPos;
layout(location = 1) out uvec4 outCellInfo;
layout(location = 2) out float outSharpness;
layout(location = 3) out vec2 outCellCoord;
layout(location = 4) out vec4 outColour;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
    GlyphInfo gi = glyphBuffer.glyphs[inGlyphIndex];

    vec2 pos[4];
	pos[0] = vec2(inRect.x, inRect.y);
	pos[1] = vec2(inRect.z, inRect.y);
	pos[2] = vec2(inRect.x, inRect.w);
	pos[3] = vec2(inRect.z, inRect.w);

    vec2 glyphPos[4];
    glyphPos[0] = vec2(gi.bbox.x, gi.bbox.y);
    glyphPos[1] = vec2(gi.bbox.z, gi.bbox.y);
    glyphPos[2] = vec2(gi.bbox.x, gi.bbox.w);
    glyphPos[3] = vec2(gi.bbox.z, gi.bbox.w);

    vec2 cellCoord[4];
    cellCoord[0] = vec2(0.0f, 0.0f);
    cellCoord[1] = vec2(gi.cellInfo.z, 0.0f);
    cellCoord[2] = vec2(0.0f, gi.cellInfo.w);
    cellCoord[3] = vec2(gi.cellInfo.z, gi.cellInfo.w);

    gl_Position = vec4(pos[gl_VertexIndex], 0.0f, 1.0f);
    outGlyphPos = glyphPos[gl_VertexIndex];
    outCellInfo = gi.cellInfo;
    outSharpness = inSharpness;
    outCellCoord = cellCoord[gl_VertexIndex];
	outColour = inColour;
}
