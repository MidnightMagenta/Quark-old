#include "../include/GL_assets.hpp"

bool qrk::assets::Program::Compile(const std::string &vertexPath,
                                   const std::string &fragmentPath) {
    if (!std::filesystem::exists(vertexPath)) {
        std::string error = "Invalid vertex shader path: " + vertexPath;
        qrk::debug::LogError(error);
        qrk::debug::ShowErrorBox(error);
        return false;
    }
    if (!std::filesystem::exists(fragmentPath)) {
        std::string error = "Invalid fragment shader path: " + fragmentPath;
        qrk::debug::LogError(error);
        qrk::debug::ShowErrorBox(error);
        return false;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vertexShaderFile(vertexPath);
    std::ifstream fragmentShaderFile(fragmentPath);

    if (vertexShaderFile.is_open()) {
        std::stringstream vertexBuffer;
        vertexBuffer << vertexShaderFile.rdbuf();
        vertexCode = vertexBuffer.str();
        vertexShaderFile.close();
    } else
        return false;

    if (fragmentShaderFile.is_open()) {
        std::stringstream fragmentBuffer;
        fragmentBuffer << fragmentShaderFile.rdbuf();
        fragmentCode = fragmentBuffer.str();
        fragmentShaderFile.close();
    } else
        return false;

    char const *sourcePtr = vertexCode.c_str();
    glShaderSource(vertexShader, 1, &sourcePtr, NULL);
    glCompileShader(vertexShader);

    sourcePtr = fragmentCode.c_str();
    glShaderSource(fragmentShader, 1, &sourcePtr, NULL);
    glCompileShader(fragmentShader);

    GLint compileStatus = GL_FALSE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(vertexShader, 1024, &log_length, message);
        std::stringstream error;
        error << "Failed to compile vertex shader: " << message;
        qrk::debug::LogError(error.str());
        qrk::debug::ShowErrorBox(error.str());
        return false;
    }

    glGetShaderiv(vertexShader, GL_VALIDATE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(vertexShader, 1024, &log_length, message);
        std::stringstream error;
        error << "Failed to compile vertex shader: " << message;
        qrk::debug::LogError(error.str());
        qrk::debug::ShowErrorBox(error.str());
        return false;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(fragmentShader, 1024, &log_length, message);
        std::stringstream error;
        error << "Failed to compile fragment shader: " << message;
        qrk::debug::LogError(error.str());
        qrk::debug::ShowErrorBox(error.str());
        return false;
    }

    glGetShaderiv(fragmentShader, GL_VALIDATE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(fragmentShader, 1024, &log_length, message);
        std::stringstream error;
        error << "Failed to compile fragment shader: " << message;
        qrk::debug::LogError(error.str());
        qrk::debug::ShowErrorBox(error.str());
        return false;
    }

    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    glGetProgramiv(programHandle, GL_LINK_STATUS, &compileStatus);

    if (compileStatus != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(programHandle, 1024, &log_length, message);
        std::stringstream error;
        error << "Failed to link program: " << message;
        qrk::debug::LogError(error.str());
        qrk::debug::ShowErrorBox(error.str());
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}