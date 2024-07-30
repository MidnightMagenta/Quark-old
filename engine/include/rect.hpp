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
    Rect();
    Rect(const qrk::vec2f &_size);

    void SetSize(float x, float y) { this->size = qrk::vec2f({x, y}); }
    qrk::vec2f GetSize() { return this->size; }

    void SetPosition(float x, float y) { this->position = qrk::vec2f({x, y}); }
    qrk::vec2f GetPosition() { return this->position; }

    void SetOffset(float x, float y) { this->offset = qrk::vec2f({x, y}); }
    qrk::vec2f GetOffset() { return this->offset; }

    void SetRotation(float _rotation) { this->rotation = _rotation; }
    float GetRotation() { return this->rotation; }

    void SetColor(qrk::Color _color) { color = _color; }
    qrk::Color GetColor() { return this->color; }

    void SetTexture(qrk::Texture2D &_texture) { texture = &_texture; }
    void RemoveTexture() { texture = nullptr; }

    /// Set z layer betwen -1 and 1
    void SetLayer(const float layer) {
        zLayer = -std::clamp(layer, -0.999f, 0.999f);
    }

    qrk::DrawData_2D GetDrawData();

private:
    qrk::vec2f size;
    qrk::vec2f position;
    qrk::vec2f offset;
    float rotation;
    float zLayer = 0.f;
    qrk::Color color;
    qrk::Texture2D *texture;

    const std::vector<GLfloat> vertices = {
            1,  1,  1, 0, -1, 1,  0, 0, -1, -1, 0, 1,
            -1, -1, 0, 1, 1,  -1, 1, 1, 1,  1,  1, 0,
    };

    GLuint VAO;
    GLuint VBO;
};
}// namespace qrk

#endif// !Q_RECT
