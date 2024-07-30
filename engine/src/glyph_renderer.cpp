#include "../include/glyph_renderer.hpp"
#include "../include/stb_truetype.hpp"

qrk::Font::Font(std::string path, int fontSize, int bitmapSize, int _firstGlyph,
                int _glyphCount) {
    //declare storage buffers
    unsigned char *ttfBuffer = new unsigned char[1 << 20];
    unsigned char *bitmap = new unsigned char[bitmapSize * bitmapSize];
    //open the font file
    if (!std::filesystem::exists(path)) {
        qrk::debug::Error("File does not exist. Path: " + path,
                          qrk::debug::Q_FAILED_TO_FIND_FILE);
    }
    FILE *f = nullptr;
    fopen_s(&f, path.c_str(), "rb");
    if (f == nullptr) {
        qrk::debug::Error("Failed to open file at: " + path,
                          qrk::debug::Q_FAILED_TO_LOAD_FONT);
    }
    fread_s(ttfBuffer, 1 << 20, 1, 1 << 20, f);

    //read the font into a bitmap
    stbtt_packedchar *charData = new stbtt_packedchar[_glyphCount];
    int result = 0;
    stbtt_pack_context context;
    result = stbtt_PackBegin(&context, bitmap, bitmapSize, bitmapSize, 0, 16,
                             nullptr);
    if (result == 0) {
        qrk::debug::Error("Failed to load font",
                          qrk::debug::Q_FAILED_TO_LOAD_FONT);
    }
    result = stbtt_PackFontRange(&context, ttfBuffer, 0, (float) fontSize,
                                 _firstGlyph, _glyphCount, charData);
    if (result == 0) {
        qrk::debug::Error("Failed to load font",
                          qrk::debug::Q_FAILED_TO_LOAD_FONT);
    }
    stbtt_PackEnd(&context);
    delete[] ttfBuffer;
    this->texture_bitmap.LoadFromData(bitmap, bitmapSize, bitmapSize,
                                      Q_TEXT_TEXTURE);

    //initialize font
    glyphs = new GlyphData[_glyphCount];
    firstGlyph = _firstGlyph;
    glyphCount = _glyphCount;
    fontHeight = fontSize;

    for (int i = 0; i < _glyphCount; i++) {
        stbtt_aligned_quad quad;
        float x = 0, y = 0;
        stbtt_GetPackedQuad(charData, texture_bitmap.width,
                            texture_bitmap.height, i, &x, &y, &quad, 1);

        GlyphData &data = glyphs[i];
        data.point_0 = qrk::vec2f({quad.x0, quad.y0});
        data.point_1 = qrk::vec2f({quad.x1, quad.y1});
        data.texture_0 = qrk::vec2f({quad.s0, quad.t0});
        data.texture_1 = qrk::vec2f({quad.s1, quad.t1});
        data.shift = x;
    }
    delete[] charData;
    texture.LoadFromImage(texture_bitmap);
}

qrk::DrawData_Text qrk::Text::GetDrawData() {
    std::vector<GLfloat> vertexArray;
    float totalShift = 0;

    for (int i = 0; i < text.size(); i++) {
        qrk::Font::GlyphData *glyph = font->GetGlyphData(text[i]);
        //vertex 0
        vertexArray.push_back(glyph->point_1.x() + totalShift + position.x() * 2);
        vertexArray.push_back(glyph->point_0.y() + font->fontHeight / 2 + position.y() * 2);
        vertexArray.push_back(glyph->texture_1.x());
        vertexArray.push_back(glyph->texture_0.y());
        //vertex 1
        vertexArray.push_back(glyph->point_0.x() + totalShift + position.x() * 2);
        vertexArray.push_back(glyph->point_0.y() + font->fontHeight / 2 + position.y() * 2);
        vertexArray.push_back(glyph->texture_0.x());
        vertexArray.push_back(glyph->texture_0.y());
        //vertex 2
        vertexArray.push_back(glyph->point_0.x() + totalShift + position.x() * 2);
        vertexArray.push_back(glyph->point_1.y() + font->fontHeight / 2 + position.y() * 2);
        vertexArray.push_back(glyph->texture_0.x());
        vertexArray.push_back(glyph->texture_1.y());
        //vertex 3
        vertexArray.push_back(glyph->point_0.x() + totalShift + position.x() * 2);
        vertexArray.push_back(glyph->point_1.y() + font->fontHeight / 2 + position.y() * 2);
        vertexArray.push_back(glyph->texture_0.x());
        vertexArray.push_back(glyph->texture_1.y());
        //vertex 4
        vertexArray.push_back(glyph->point_1.x() + totalShift + position.x() * 2);
        vertexArray.push_back(glyph->point_1.y() + font->fontHeight / 2 + position.y() * 2);
        vertexArray.push_back(glyph->texture_1.x());
        vertexArray.push_back(glyph->texture_1.y());
        //vertex 5
        vertexArray.push_back(glyph->point_1.x() + totalShift + position.x() * 2);
        vertexArray.push_back(glyph->point_0.y() + font->fontHeight / 2 + position.y() * 2);
        vertexArray.push_back(glyph->texture_1.x());
        vertexArray.push_back(glyph->texture_0.y());

        totalShift += glyph->shift + spacing;
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(GLfloat), vertexArray.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    qrk::DrawData_Text drawData;
    drawData.VAO = VAO;
    drawData.VBO = VBO;
    drawData.color = qrk::ConvertToFloat(color);
    drawData.texture = &font->texture;
    drawData.vertexCount = 6 * text.size();
    drawData.zLayer = zLayer;
    return drawData;
}