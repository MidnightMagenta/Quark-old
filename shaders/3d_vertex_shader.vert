#version 460 core

struct Material
{
	float shininess;
	vec3 specular;
	vec3 diffuse;
	vec3 ambient;
};

layout (location = 0) in vec4 vertLocation;
layout (location = 1) in vec2 textureLoaction;
layout (location = 2) in vec3 normalLoaction;

out vec3 f_normals;
out vec2 f_textures;
out vec4 f_color;
out vec4 f_transformedVertices;
out vec3 f_cameraPosition;
out Material f_material;

layout(std140, row_major) uniform uniformBlock{
	mat4 position;
	mat4 rotation;
	mat4 scale;
	mat4 view;
	mat4 projection;
	vec4 color;
	vec3 cameraPosition;
	Material material;
};

void main()
{
	vec4 vertexTransformed = projection * view * position * rotation * scale * vertLocation;
	gl_Position = vertexTransformed;
	f_transformedVertices = vertexTransformed;
	mat3 normalMatrix = mat3(projection * view * rotation * scale);
	normalMatrix = transpose(inverse(normalMatrix));

	f_normals = normalize(normalMatrix * normalLoaction);
	f_textures = textureLoaction;
	f_color = color;
	f_cameraPosition = cameraPosition;
	f_material = material;
}