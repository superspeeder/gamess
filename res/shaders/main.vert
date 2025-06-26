#version 460 core

layout(location = 0) in vec2 positionIn;
layout(location = 1) in vec2 textureCoordsIn;

out vec2 fTextureCoords;

void main() {
    gl_Position = vec4(positionIn, 0.0, 1.0);
    fTextureCoords = textureCoordsIn;
}

