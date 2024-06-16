#ifndef Q_DRAW
#define Q_DRAW

#include "../include/GL_assets.hpp"
#include "../include/constants.hpp"
#include "../include/matrix.hpp"
#include "../include/vector.hpp"
#include "../include/window.hpp"
#include "glad/glad.h"
#include <vector>

namespace qrk {
enum class DrawTarget { Q_3D_DRAW, Q_2D_DRAW };

struct obj {
    GLuint VAO = 0;
    GLuint VBO = 0;

    GLsizei vertexCount = 0;

    vec3f position = qrk::vec3f({0.f, 0.f, 0.f});
    vec3f rotation = qrk::vec3f({0.f, 0.f, 0.f});
    vec3f scale = qrk::vec3f({1.f, 1.f, 1.f});
    ColorF color = qrk::ColorF(1.f, 1.f, 1.f, 1.f);
};

struct settings {
    bool depthTest = true;
    bool cullFaces = true;
    bool alpha = true;
    bool multisample = true;
    float fov = 70 * qrk::units::deg;
};

class Renderer {
public:
    Renderer(qrk::glWindow &_targetWindow, qrk::settings *_settings = nullptr)
        : targetWindow(&_targetWindow), __settings(_settings) {

        if (_settings != nullptr) {
            if (_settings->depthTest == true) {
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);
            } else {
                glDisable(GL_DEPTH_TEST);
            }
            if (_settings->cullFaces == true) {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
            } else {
                glDisable(GL_CULL_FACE);
            }
            if (_settings->alpha == true) {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            } else {
                glDisable(GL_BLEND);
            }
            if (_settings->multisample == true) {
                glEnable(GL_MULTISAMPLE);
                glSampleCoverage(1, GL_FALSE);
            } else {
                glDisable(GL_MULTISAMPLE);
            }
        } else {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_MULTISAMPLE);
            glSampleCoverage(1, GL_FALSE);
        }

        q_3dDraw = qrk::assets::Program("shaders/3d_vertex_shader.vert",
                                        "shaders/3d_fragment_shader.frag");
    }

    ~Renderer() {}

    void QueueDraw(const obj &drawData, DrawTarget target) {
        switch (target) {
            case qrk::DrawTarget::Q_3D_DRAW:
                q_3dObjects.push_back(drawData);
                break;
            case qrk::DrawTarget::Q_2D_DRAW:
                //temporairly empty until 2d draw logic is done
                break;
            default:
                break;
        }
    }

    void Draw();

private:
    //vectors containing draw queue
    std::vector<obj> q_3dObjects;

    //programs for different draw types
    qrk::assets::Program q_3dDraw;

    //misc variables
    qrk::glWindow *targetWindow;
    qrk::settings *__settings;
};
}// namespace qrk

#endif// !Q_DRAW