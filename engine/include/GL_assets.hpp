#ifndef QRK_GL_ASSETS
#define QRK_GL_ASSETS

#include "../glad/glad.h"
#include "../include/qrk_debug.hpp"
#include <Windows.h>
#include <filesystem>
#include <string>

namespace qrk::assets {

inline GLuint boundProgramID = 0;

class Program {
public:
    Program() : programHandle(NULL), uniformBlockIndex(NULL) {}
    Program(const std::string &vertexPath, const std::string &fragmentPath) {
        programHandle = glCreateProgram();
        if (!Compile(vertexPath, fragmentPath)) {
            qrk::debug::Error(
                    "Failed to compile shader. Vertex path: " + vertexPath +
                            " Fragment shader: " + fragmentPath,
                    qrk::debug::Q_FAILED_TO_COMPILE_SHADER);
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
    bool Compile(const std::string &vertexPath,
                 const std::string &fragmentPath);
};
}// namespace qrk::assets
namespace qrk {
inline GLuint GetBoundProgram() { return qrk::assets::boundProgramID; }
inline void UnbindProgram() {
    glUseProgram(0);
    qrk::assets::boundProgramID = 0;
}
}// namespace qrk

#endif// !QRK_GL_ASSETS
