#version 330

layout (location = 0) in vec3 position;

uniform mat4 translate;
uniform mat4 scale;

void main() {
    gl_Position = translate * scale * vec4(position, 1.0);
}