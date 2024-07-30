#version 460 core

layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec2 texturePosition;

layout (std140, row_major) uniform uniformBlock {
    vec4 color;
    vec2 screenSize;
    float zLayer;
};

out vec4 f_color;
out vec2 f_texturePosition;

void main(){
    gl_Position = vec4((vertexPosition.x - (screenSize.x)) / screenSize.x, (-vertexPosition.y + screenSize.y) / screenSize.y, zLayer, 1);

    f_color = color;
    f_texturePosition = texturePosition;
}