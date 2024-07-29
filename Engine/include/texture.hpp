#ifndef Q_TEXTURE
#define Q_TEXTURE

#include "../dependencies/glad/glad.h"
#include "../include/image.hpp"
#include "../include/qrk_debug.hpp"
#include <string>

#define Q_TEXT_TEXTURE 5

namespace qrk {
struct Texture2DSettings {
    GLuint nearFilter = GL_LINEAR;
    GLuint farFilter = GL_LINEAR;
    GLuint wrap_s = GL_REPEAT;
    GLuint wrap_t = GL_REPEAT;
};

class Image {
public:
    Image() : imageData(nullptr), width(0), height(0), channels(0) {}
    explicit Image(const std::string &path) { LoadFromFile(path); }
    ~Image() { DeleteImage(); }
    void LoadFromFile(const std::string &path);
    void LoadFromData(unsigned char *data, int _width, int _height,
                      int _channels);
    void DeleteImage() {
        stbi_image_free(imageData);
        imageData = nullptr;
    }

    unsigned char *imageData;
    int width;
    int height;
    int channels;
};
class Texture2D {
public:
    Texture2D() : texture(0) {}
    Texture2D(const std::string &path,
              const qrk::Texture2DSettings &settings = {GL_LINEAR, GL_LINEAR,
                                                        GL_REPEAT, GL_REPEAT})
        : texture(0) {
        LoadFromFile(path);
    }
    ~Texture2D() { DeleteTexture(); }
    void LoadFromFile(const std::string &path,
                      const qrk::Texture2DSettings &settings = {
                              GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT});
    void LoadFromImage(qrk::Image &image,
                       const qrk::Texture2DSettings &settings = {
                               GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT});
    void DeleteTexture();
    void BindTexture();

private:
    GLuint texture;
};
}// namespace qrk

#endif// !Q_TEXTURE