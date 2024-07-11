#version 460 core

in vec2 f_texturePos;
in vec4 f_color;

out vec4 finalColor;

uniform bool textured;
uniform sampler2D f_texture;

void main()
{
	if(textured) { finalColor = texture(f_texture, f_texturePos) * f_color; }
	else { finalColor = f_color; }
}