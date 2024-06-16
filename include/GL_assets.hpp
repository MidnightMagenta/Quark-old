#ifndef QRK_GL_ASSETS
#define QRK_GL_ASSETS

#include "../glad/glad.h"
#include "../include/qrk_debug.hpp"
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace qrk::assets {
class Program {
public:
    Program() : programHandle(NULL){}
    Program(std::string vertexPath, std::string fragmentPath) {
        programHandle = glCreateProgram();
        if (!Compile(vertexPath, fragmentPath)) {
            qrk::Debug::LogError(
                    "Failed to compile shader. Vertex path: " + vertexPath +
                    " Fragment shader: " + fragmentPath);
            throw std::exception();
        }
    }

    GLuint programHandle;

private:
    bool Compile(std::string vertexPath, std::string fragmentPath);
};
}// namespace qrk::assets

#endif// !QRK_GL_ASSETS
