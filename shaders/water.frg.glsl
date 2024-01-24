#version 450

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNor;
layout (location = 2) in vec2 vTex;

uniform vec2 atlasCoords;

out vec4 fragColor;

void main() {
    fragColor = vec4(0.4, 0.4, 0.8, 0.5);
}