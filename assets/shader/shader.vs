#version 330

layout (location = 0) in vec3 position;

uniform mat4 translate;

void main() {
    gl_Position = translate * vec4(position, 1.0);
}