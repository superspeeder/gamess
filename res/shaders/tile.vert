#version 460 core

layout(location = 0) in vec2 positionIn;
layout(location = 1) in uvec2 localTilePos;
layout(location = 2) in uint tileId;

out vec2 fTextureCoords;
flat out uint fTileId;

uniform uvec2 uAtlasSize;
uniform mat4 uTransformMatrix;

uniform vec2 uChunkOffset;

void main() {
    gl_Position = uTransformMatrix * vec4(uChunkOffset + positionIn + localTilePos, 0.0, 1.0);

    if (tileId == 0) {
        gl_Position = vec4(0.0,0.0,0.0,0.0);
        return; // tile id 0 should not be rendered, so we cause degenerate triangles to be generated instead.
    }

    uint realTileId = tileId - 1;
    fTileId = realTileId;

    vec2 tileSize = vec2(1.0 / float(uAtlasSize.x), 1.0 / float(uAtlasSize.y));
    float row = float(realTileId / uAtlasSize.x) + positionIn.x;
    float column = float(realTileId % uAtlasSize.x) + positionIn.y; // we use the same number to avoid any issues
    fTextureCoords = vec2(column, row) * tileSize;
}

