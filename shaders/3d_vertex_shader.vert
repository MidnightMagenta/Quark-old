#version 460 core

layout (location = 0) in vec4 vertLocation;
layout (location = 1) in vec2 textureLoaction;
layout (location = 2) in vec3 normalLoaction;

out vec3 normalsToFrag;
out vec2 texturesToFrag;

uniform mat4 posMatrix;
uniform mat4 rotMatrix;
uniform mat4 scaleMatrix;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;

void main()
{
	gl_Position = projMatrix * viewMatrix * scaleMatrix * posMatrix * rotMatrix * vertLocation;

	normalsToFrag = normalLoaction;
	texturesToFrag = textureLoaction;
}