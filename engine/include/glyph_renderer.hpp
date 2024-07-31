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
    Font(std::string path, int fontSize, int bitmapSize = 1000,
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
public:
    Text() = default;
    explicit Text(qrk::Font &_font) {
        font = &_font;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }
    ~Text() = default;

    void SetText(const std::string &_text) {
        text = _text;
        float _textWidth = 0;
        for (int i = 0; i < text.size(); i++) {
            qrk::Font::GlyphData *glyph = font->GetGlyphData(text[i]);
            _textWidth += glyph->shift + spacing;
        }
        _textWidth /= 2;
        textWidth = std::round(_textWidth);
    }
    std::string GetText() { return text; }

    int GetWidth() const { return textWidth; }
    int GetHeight() const { return font->fontHeight / 3; }

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
    int textWidth = 0;

    qrk::vec2f position = qrk::vec2f({0, 0});
    qrk::Color color = {255, 255, 255, 255};
    float zLayer = 0.f;
};
}// namespace qrk

#endif// !Q_GLYPH_RENDERER
