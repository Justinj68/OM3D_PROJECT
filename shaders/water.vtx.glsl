#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

uniform mat4 view;
uniform mat4 projection;

layout (location = 0) out vec3 vPos;
layout (location = 1) out vec3 vNor;
layout (location = 2) out vec2 vTex;

void main() {
    vPos = aPos;
    vNor = vec3(0.0,1.0,0.0);
    vTex = aTex;
    gl_Position = projection * view * vec4(aPos, 1.0);
}