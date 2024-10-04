#version 330

layout (location = 0) in vec3 position;

uniform mat4 world;

const vec4 colors[3] = vec4[3]( vec4(1, 0, 0, 1),
                                vec4(0, 1, 0, 1),
                                vec4(0, 0, 1, 1) );

out vec4 color;

void main() {
    color = colors[gl_VertexID];
    gl_Position = world * vec4(position, 1.0);
}