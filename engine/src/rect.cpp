#include "../include/rect.hpp"

qrk::Rect::Rect() {
    size = qrk::vec2f({100, 100});
    offset = qrk::vec2f({-50, -50});
    position = qrk::vec2f({0, 0});
    rotation = 0.f;
    color = {255, 255, 255, 255};
    texture = nullptr;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                 vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

qrk::Rect::Rect(const qrk::vec2f &_size) {
    size = _size;
    offset = qrk::vec2f({-size.x() / 2, -size.y() / 2});
    position = qrk::vec2f({0, 0});
    rotation = 0.f;
    color = {255, 255, 255, 255};
    texture = nullptr;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                 vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

qrk::DrawData_2D qrk::Rect::GetDrawData() {
    DrawData_2D data;
    data.VAO = this->VAO;
    data.VBO = this->VBO;
    data.position = qrk::vec2f({this->position.x() - this->offset.x(),
                                this->position.y() - this->offset.y()});
    data.size = this->size;
    data.color = qrk::ConvertToFloat(this->color);
    data.texture = this->texture;
    data.rotation = this->rotation;
    data.vertexCount = this->vertices.size() / 4;
    data.zLayer = zLayer;
    return data;
}