#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;
layout (location = 3) in vec3 color;

uniform mat4 mvp;

out vec3 interpolated_normal;
out vec2 interpolated_texcoord;

out vec4 interpolated_color;

void main() {
    interpolated_normal = normal;
    interpolated_texcoord = texcoord;
    interpolated_color = vec4(color, 1.0);
    gl_Position = mvp * vec4(position, 1.0);
}