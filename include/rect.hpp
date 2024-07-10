#ifndef Q_RECT
#define Q_RECT

#include "../include/color.hpp"
#include "../include/draw.hpp"
#include "../include/texture.hpp"
#include "../include/vector.hpp"
#include <vector>

namespace qrk {
class Rect {
public:
    Rect() {
        size = qrk::vec2f({100, 100});
        originOffset = qrk::vec2f({0, 0});
        position = qrk::vec2f({0, 0});
        rotation = 0.f;
        color = {255, 255, 255, 255};
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                     vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    void SetSize(float x, float y) { this->size = qrk::vec2f({x, y}); }
    qrk::vec2f GetSize() { return this->size; }
    void SetPosition(float x, float y) {
        this->position = qrk::vec2f({x, y});
    }
    qrk::vec2f GetPosition() { return this->position; }
    void SetOrigin(float x, float y) {
        this->originOffset = qrk::vec2f({x, y});
    }
    qrk::vec2f GetOrigin() { return this->originOffset; }
    void SetRotation(float _rotation) { this->rotation = _rotation; }
    float GetRotation() { return this->rotation; }
    void SetColor(qrk::Color _color) { color = _color; }
    qrk::Color GetColor() { return this->color; }

    qrk::DrawData_2D GetDrawData();

private:
    qrk::vec2f size;
    qrk::vec2f position;
    qrk::vec2f originOffset;
    float rotation;
    qrk::Color color;

    GLuint VAO;
    GLuint VBO;

    const std::vector<GLfloat> vertices = {
            1,  -1, 1, 0, 1, 1,  1, 1, -1, 1, 0, 1,

            -1, -1, 0, 0, 1, -1, 1, 0, -1, 1, 0, 1,
    };
};
}// namespace qrk

#endif// !Q_RECT