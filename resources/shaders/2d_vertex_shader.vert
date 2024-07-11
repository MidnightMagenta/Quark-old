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
    vec4 vPos = vec4(vertexPos * size, 0, 1);
    vPos = rotation * vPos;
    gl_Position = vPos + vec4(position, 0, 0);
}