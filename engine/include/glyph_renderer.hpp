#ifndef Q_GLYPH_RENDERER
#define Q_GLYPH_RENDERER

#include "../include/draw.hpp"
#include "../include/texture.hpp"
#include "../include/vector.hpp"

namespace qrk {
class Font {
public:
    struct GlyphData {
        qrk::vec2f point_0;  //top left
        qrk::vec2f point_1;  //bottom right
        qrk::vec2f texture_0;//top left
        qrk::vec2f texture_1;//bottom right
        float shift;
    };

public:
    Font() = default;
    Font(std::string path, int fontSize, int bitmapSize = 400,
         int _firstGlyph = 32, int _glyphCount = 95);
    ~Font() {
        texture_bitmap.DeleteImage();
        texture.DeleteTexture();
        delete[] glyphs;
    }
    GlyphData *GetGlyphData(char glyph) {
        if (glyph >= glyphCount + firstGlyph) {
            qrk::debug::Error("Invalid glyph", qrk::debug::Q_INVALID_ENUM);
        }
        if (glyph < firstGlyph) {
            qrk::debug::Error("Invalid glyph", qrk::debug::Q_INVALID_ENUM);
        }

        return &glyphs[glyph - firstGlyph];
    }
    //private:
    GlyphData *glyphs = nullptr;
    qrk::Image texture_bitmap;
    qrk::Texture2D texture;

    int firstGlyph;
    int glyphCount;
    int fontHeight;
};
class Text {
    //public:
    // class Glyph {
    // public:
    //     Glyph() {
    //         glGenVertexArrays(1, &VAO);
    //         glGenBuffers(1, &VBO);
    //         glBindVertexArray(VAO);
    //         glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //         std::vector<GLfloat> vertexData = ConvertToGlArray();
    //         glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat),
    //                      vertexData.data(), GL_STATIC_DRAW);
    //     }
    //
    // private:
    //     std::vector<GLfloat> ConvertToGlArray() {
    //         std::vector<GLfloat> vertexData;
    //         for (int i = 0; i < vertices.size(); i++) {
    //             vertexData.push_back(vertices[i].x());
    //             vertexData.push_back(vertices[i].y());
    //             vertexData.push_back(textures[i].x());
    //             vertexData.push_back(textures[i].y());
    //         }
    //         return vertexData;
    //     }
    //
    //     GLuint VAO = 0;
    //     GLuint VBO = 0;
    //     qrk::vec2f position;
    //     qrk::vec2f size;
    //
    //     std::array<qrk::vec2f, 6> vertices = {
    //             qrk::vec2f({1, 1}),   qrk::vec2f({-1, 1}), qrk::vec2f({-1, -1}),
    //             qrk::vec2f({-1, -1}), qrk::vec2f({1, -1}), qrk::vec2f({1, 1}),
    //     };
    //     std::array<qrk::vec2f, 6> textures = {
    //             qrk::vec2f({1, 0}), qrk::vec2f({0, 0}), qrk::vec2f({0, 1}),
    //             qrk::vec2f({0, 1}), qrk::vec2f({1, 1}), qrk::vec2f({1, 0}),
    //     };
    // };

public:
    Text() = default;
    explicit Text(qrk::Font &_font) {
        font = &_font;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }
    ~Text() = default;

    void SetText(const std::string &_text) { text = _text; }
    std::string GetText() { return text; }

    void SetColor(const qrk::Color &_color) { color = _color; }
    qrk::Color GetColor() const { return color; }

    void SetPosition(const float x, const float y) {
        position = qrk::vec2f({x, y});
    }
    qrk::vec2f GetPosition() { return position; }

    void SetSpacing(int _spacing) { spacing = _spacing; }

    /// Set z layer betwen -1 and 1
    void SetLayer(const float layer) {
        zLayer = -std::clamp(layer, -0.999f, 0.999f);
    }

    qrk::DrawData_Text GetDrawData();

private:
    GLuint VAO = 0;
    GLuint VBO = 0;

    qrk::Font *font;
    float spacing = 0;
    std::string text = "";

    qrk::vec2f position = qrk::vec2f({0, 0});
    qrk::Color color = {255, 255, 255, 255};
    float zLayer = 0.f;
};
}// namespace qrk

#endif// !Q_GLYPH_RENDERER
