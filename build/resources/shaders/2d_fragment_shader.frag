#version 460 core

in vec2 f_texturePos;
in vec4 f_color;

out vec4 finalColor;

uniform bool textured;
uniform sampler2D f_texture;

void main()
{
	if(textured) { finalColor = texture(f_texture, f_texturePos) * vec4(f_color.x, f_color.z, f_color.y, f_color.w); }
	else { finalColor = f_color; }
}