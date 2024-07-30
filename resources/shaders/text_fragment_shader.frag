#version 460 core

in vec4 f_color;
in vec2 f_texturePosition;

uniform sampler2D f_texture;

out vec4 color;

void main(){
    color = texture(f_texture, f_texturePosition) * vec4(f_color.x, f_color.z, f_color.y, f_color.w);
}