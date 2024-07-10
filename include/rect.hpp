#ifndef Q_RECT
#define Q_RECT

#include "../include/draw.hpp"
#include "../include/texture.hpp"
#include "../include/vector.hpp"
#include "../include/draw.hpp"
#include "../include/color.hpp"
#include <vector>

namespace qrk {
class Rect {
public:
    Rect() {
        size = qrk::vec2f({100, 100});
        position = qrk::vec2f({0, 0});
        rotation = 0.f;
        color = {255, 255, 255, 255};
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(VBO, vertices.size() * sizeof(GLfloat), vertices.data(),
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    void SetSize(float x, float y) { this->size = qrk::vec2f({x, y}); }
    qrk::vec2f GetSize() { return this->size; }
    void SetPosition(float x, float y) { this->position = qrk::vec2f({x, y}); }
    qrk::vec2f GetPosition() { return this->position; }
    void SetRotation(float _rotation) { this->rotation = _rotation; }
    float GetRotation() { return this->rotation; }
    void SetColor(qrk::Color _color) { color = _color; }
    qrk::Color GetColor() { return this->color; }

    qrk::DrawData_2D GetDrawData();

private:
    qrk::vec2f size;
    qrk::vec2f position;
    float rotation;
    qrk::Color color;

    const std::vector<GLfloat> vertices = {0, 0,  0, 1, 1, 0,  1, 1,
                                           0, -1, 0, 0, 1, -1, 1, 0,
                                           1, 0,  1, 1, 0, -1, 0, 0};

    GLuint VAO;
    GLuint VBO;
};
}// namespace qrk

#endif// !Q_RECT
