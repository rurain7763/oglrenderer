#version 330

layout (location = 0) in vec3 position;

uniform float scale;

void main() {
    gl_Position = vec4(position.x * scale, position.y * scale, position.z * scale, 1.0);
}