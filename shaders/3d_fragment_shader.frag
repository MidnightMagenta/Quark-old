#version 460 core

//uniform sampler2D inTexture;
//uniform bool textured;

in vec3 normalsToFrag;
in vec2 texturesToFrag;
in vec4 colorToFrag;

out vec4 outColor;

void main()
{
	outColor = colorToFrag;
}