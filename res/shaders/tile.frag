#version 460 core

in vec2 fTextureCoords;
in uint fTileId;

out vec4 colorOut;

uniform sampler2D uTexture;

void main() {
    colorOut = texture(uTexture, vec2(fTextureCoords.x, 1.0 - fTextureCoords.y));
//    colorOut = vec4(1.0, 1.0, 1.0, 1.0);
}