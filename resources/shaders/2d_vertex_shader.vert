#version 460 core

layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 texturePos;

layout(std140, row_major) uniform uniformBlock{
	mat4 rotation;
    mat4 position;
    mat4 size;
    vec4 color;
};

out vec2 f_texturePos;
out vec4 f_color;

void main()
{
    f_texturePos = texturePos;
    f_color = color;
    vec4 finalPos = vec4(vertexPos, 0, 1);
    finalPos = rotation * position * size * finalPos;
    gl_Position = finalPos;
}