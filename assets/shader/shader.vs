#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 mvp;

out vec4 interpolated_color;

void main() {
    interpolated_color = vec4(color, 1.0);
    gl_Position = mvp * vec4(position, 1.0);
}