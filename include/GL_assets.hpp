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

inline GLuint boundProgramID = 0;

class Program {
public:
    Program() : programHandle(NULL), uniformBlockIndex(NULL) {}
    Program(std::string vertexPath, std::string fragmentPath) {
        programHandle = glCreateProgram();
        if (!Compile(vertexPath, fragmentPath)) {
            qrk::Debug::LogError(
                    "Failed to compile shader. Vertex path: " + vertexPath +
                    " Fragment shader: " + fragmentPath);
            throw std::exception();
        }
        glUseProgram(programHandle);
        uniformBlockIndex =
                glGetUniformBlockIndex(programHandle, "uniformBlock");
    }

    void UseProgram() {
        glUseProgram(this->programHandle);
        boundProgramID = this->programHandle;
    }

    GLuint programHandle;
    GLuint uniformBlockIndex;

private:
    bool Compile(std::string vertexPath, std::string fragmentPath);
};
}// namespace qrk::assets
namespace qrk {
inline GLuint GetBoundProgram() { return qrk::assets::boundProgramID; }
}// namespace qrk

#endif// !QRK_GL_ASSETS
