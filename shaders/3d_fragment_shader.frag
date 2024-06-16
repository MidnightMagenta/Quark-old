#version 460 core

uniform vec4 color;
//uniform sampler2D inTexture;
//uniform bool textured;

in vec3 normalsToFrag;
in vec2 texturesToFrag;

out vec4 outColor;

void main()
{
	outColor = color;
}