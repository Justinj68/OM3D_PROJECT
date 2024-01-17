#version 450

layout (location = 0) in vec3 normal;

layout (binding = 0) uniform sampler2D atlas;

out vec4 fragColor;

void main() {
    fragColor = vec4((normal + 1.0) / 2.0, 1.0);
}