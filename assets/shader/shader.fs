#version 330

uniform sampler2D texture0;

in vec3 interpolated_normal;
in vec2 interpolated_texcoord;
in vec4 interpolated_color;

out vec4 fragColor;

void main() {
    fragColor = interpolated_color * texture(texture0, interpolated_texcoord);
}