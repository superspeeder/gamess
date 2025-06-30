#version 460 core

#define BORDER_WIDTH 0.25
#define BORDER_COLOR vec4(0.0, 0.0, 0.0, 1.0)

in vec2 fTextureCoords;
in vec2 fRawTextureCoords;
flat in uint fTileId;
flat in uint fTileBordering;

out vec4 colorOut;

uniform sampler2D uTexture;

void main() {
//    bvec4((fTileBordering & uint(1)) == 1, (fTileBordering & uint(2)) == 2, (fTileBordering & uint(4)) == 4, (fTileBordering & uint(8)) == 8)
    if ((fTileBordering & uint(1)) == 1 && fRawTextureCoords.x < BORDER_WIDTH) {
        colorOut = BORDER_COLOR;
        return;
    } else if ((fTileBordering & uint(2)) == 2 && fRawTextureCoords.y < BORDER_WIDTH) {
        colorOut = BORDER_COLOR;
        return;
    } else if ((fTileBordering & uint(4)) == 4 && fRawTextureCoords.x > (1.0 - BORDER_WIDTH)) {
        colorOut = BORDER_COLOR;
        return;
    } else if ((fTileBordering & uint(8)) == 8 && fRawTextureCoords.y > (1.0 - BORDER_WIDTH)) {
        colorOut = BORDER_COLOR;
        return;
    }

    colorOut = texture(uTexture, vec2(fTextureCoords.x, 1.0 - fTextureCoords.y));
//    colorOut = vec4(1.0, 1.0, 1.0, 1.0);
}