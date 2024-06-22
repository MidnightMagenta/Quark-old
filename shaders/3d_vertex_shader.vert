#version 460 core

layout (location = 0) in vec4 vertLocation;
layout (location = 1) in vec2 textureLoaction;
layout (location = 2) in vec3 normalLoaction;

out vec3 normalsToFrag;
out vec2 texturesToFrag;
out vec4 colorToFrag;
out vec4 transformedVertexToFrag;

struct LightSource
{
	vec3 lightSourcePosition;
	vec3 lightSourceColor;
};

layout(std140, row_major) uniform uniformBlock{
	mat4 position;
	mat4 rotation;
	mat4 scale;
	mat4 view;
	mat4 projection;
	vec4 color;
};

void main()
{
	vec4 vertexTransformed = projection * view * position * rotation * scale * vertLocation;
	gl_Position = vertexTransformed;
	transformedVertexToFrag = vertexTransformed;
	mat3 normalMatrix = mat3(projection * view * rotation * scale);
	normalMatrix = transpose(inverse(normalMatrix));

	normalsToFrag = normalize(normalMatrix * normalLoaction);
	texturesToFrag = textureLoaction;
	colorToFrag = color;
}