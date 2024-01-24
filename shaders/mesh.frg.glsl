#version 450

layout (location = 0) in vec3 normal;

layout (binding = 0) uniform sampler2D atlas;

uniform bool underWater = false;

out vec4 fragColor;

void main() {
    vec3 color = vec3((normal + 1.0) / 2.0);
    if (underWater)
        color *= vec3(0.5, 0.5, 1.5);
    fragColor = vec4(color, 1.0);
}