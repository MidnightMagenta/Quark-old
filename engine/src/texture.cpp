#include "../include/texture.hpp"

void qrk::Image::LoadFromFile(const std::string &path) {
    this->imageData = stbi_load(path.c_str(), &this->width, &this->height,
                                &this->channels, NULL);
    if (!imageData) {
        qrk::debug::Error("Failed to load image: " + path,
                          qrk::debug::Q_FAILED_TO_LOAD_IMAGE);
    }
}
void qrk::Image::LoadFromData(unsigned char *data, int _width, int _height,
                              int _channels) {
    imageData = data;
    width = _width;
    height = _height;
    channels = _channels;
}

void qrk::Texture2D::LoadFromFile(const std::string &path,
                                  const qrk::Texture2DSettings &settings) {
    if (this->texture != 0) {
        qrk::debug::LogWarning("Binding texture to an existing texture object "
                               "without explicitly deleting the object");
#ifdef _DEBUG
        qrk::debug::ShowWarningBox(
                "Binding texture to an existing texture object without "
                "explicitly deleting the object");
#endif// _DEBUG
        DeleteTexture();
    }

    unsigned char *imageData;
    int width, height, channels;
    imageData = stbi_load(path.c_str(), &width, &height, &channels, NULL);

    if (!imageData) { qrk::debug::Error("Failed to load image: " + path, 10); }

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    switch (channels) {
        case STBI_grey:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED,
                         GL_UNSIGNED_BYTE, imageData);
            break;
        case STBI_grey_alpha:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG,
                         GL_UNSIGNED_BYTE, imageData);
            break;
        case STBI_rgb:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, imageData);
            break;
        case STBI_rgb_alpha:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, imageData);
            break;
        default:
            qrk::debug::Error("Invalid channel data",
                              qrk::debug::Q_FAILED_TO_LOAD_IMAGE);
    }
    stbi_image_free(imageData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, settings.wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, settings.wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, settings.nearFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, settings.farFilter);
}
void qrk::Texture2D::LoadFromImage(qrk::Image &image,
                                   const qrk::Texture2DSettings &settings) {
    if (this->texture != 0) {
        qrk::debug::Warning("Binding texture to an existing texture object "
                            "without explicitly deleting the object");
        DeleteTexture();
    }
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    switch (image.channels) {
        case STBI_grey:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, image.width, image.height, 0,
                         GL_RED, GL_UNSIGNED_BYTE, image.imageData);
            break;
        case Q_TEXT_TEXTURE:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, image.width, image.height, 0,
                         GL_RED, GL_UNSIGNED_BYTE, image.imageData);
            break;
        case STBI_grey_alpha:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, image.width, image.height, 0,
                         GL_RG, GL_UNSIGNED_BYTE, image.imageData);
            break;
        case STBI_rgb:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0,
                         GL_RGB, GL_UNSIGNED_BYTE, image.imageData);
            break;
        case STBI_rgb_alpha:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height,
                         0, GL_RGBA, GL_UNSIGNED_BYTE, image.imageData);
            break;
        default:
            qrk::debug::Error("Invalid channel data",
                              qrk::debug::Q_FAILED_TO_LOAD_IMAGE);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, settings.wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, settings.wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, settings.nearFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, settings.farFilter);
}
void qrk::Texture2D::DeleteTexture() {
    glDeleteTextures(1, &this->texture);
    this->texture = 0;
}
void qrk::Texture2D::BindTexture() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
}