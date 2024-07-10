#version 460 core

in vec2 f_texturePos;
in vec4 f_color;

out vec4 finalColor;

void main()
{
	finalColor = f_color;
}