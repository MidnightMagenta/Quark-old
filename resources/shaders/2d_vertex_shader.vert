#version 460 core

layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 texturePos;

layout(std140, row_major) uniform uniformBlock{
	mat4 rotation;
    vec2 position;
    vec2 size;
    vec4 color;
};

out vec2 f_texturePos;
out vec4 f_color;

void main()
{
    f_texturePos = texturePos;
    f_color = color;
    vec2 finalVertexPos = (vertexPos * size) + position;
    gl_Position = vec4(finalVertexPos, 0.f, 0.f);
}