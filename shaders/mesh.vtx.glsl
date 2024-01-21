#version 450

layout(location = 0) in uint packedData;

layout (location = 0) out vec3 vNor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const vec3 normals[6] = vec3[](vec3(0, 0, 1), vec3(0, 1, 0), vec3(1, 0, 0), vec3(0, 0, -1), vec3(0, -1, 0), vec3(-1, 0, 0));
const vec2 uvs[4] = vec2[](vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0));

void main() {
    int x = int(packedData & uint(63)); // 6 bits pour x
    int y = int((packedData >> 6) & uint(63)); // 6 bits pour y
    int z = int((packedData >> 12) & uint(63)); // 6 bits pour z
    int normalIndex = int((packedData >> 18) & uint(7)); // 3 bits pour la normale
    int uvIndex = int((packedData >> 21) & uint(3)); // 2 bits pour UV

    vec3 position = vec3(float(x), float(y), float(z));
    vec3 normal = normals[normalIndex];
    vec2 uv = uvs[uvIndex];

    vNor = normal;

    gl_Position = projection * view * model * vec4(position, 1.0);
}

