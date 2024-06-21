#include "../include/texture.hpp"

void qrk::Image::LoadFromFile(const std::string &path) {
    this->imageData = stbi_load(path.c_str(), &this->width, &this->height,
                                &this->channels, NULL);
    if (!imageData) {
        qrk::Debug::LogError("Failed to load image: " + path);
        qrk::Debug::ShowErrorBox("Failed to load image");
        throw std::exception();
    }
}

void qrk::Texture2D::LoadFromFile(const std::string &path,
                                  const qrk::Texture2DSettings &settings) {
    if (this->texture != 0) {
        qrk::Debug::LogWarning("Binding texture to an existing texture object "
                               "without explicitly deleting the object");
#ifdef _DEBUG
        qrk::Debug::ShowWarningBox(
                "Binding texture to an existing texture object without "
                "explicitly deleting the object");
#endif// _DEBUG
        DeleteTexture();
    }

    unsigned char *imageData;
    int width, height, channels;
    imageData = stbi_load(path.c_str(), &width, &height, &channels, NULL);

    if (!imageData) {
        qrk::Debug::LogError("Failed to load image: " + path);
        qrk::Debug::ShowErrorBox("Failed to load image");
        throw std::exception();
    }

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
            qrk::Debug::LogError("Invalid image channels: " + path);
            qrk::Debug::ShowErrorBox("Invalid image channels");
            throw std::exception();
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
        qrk::Debug::LogWarning("Binding texture to an existing texture object "
                               "without explicitly deleting the object");
#ifdef _DEBUG
        qrk::Debug::ShowWarningBox(
                "Binding texture to an existing texture object without "
                "explicitly deleting the object");
#endif// _DEBUG
        DeleteTexture();
    }
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    switch (image.channels) {
        case STBI_grey:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, image.width, image.height, 0,
                         GL_RED, GL_UNSIGNED_BYTE, image.imageData);
            break;
        case STBI_grey_alpha:
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
            qrk::Debug::LogError("Invalid image channels");
            qrk::Debug::ShowErrorBox("Invalid image channels");
            throw std::exception();
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