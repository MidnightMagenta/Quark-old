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

class obj {
public:
    GLuint VAO;
    GLuint VBO;

    vec3f &position;
    vec3f &rotation;
    vec3f &scale;
};

class Renderer {
public:
    Renderer(qrk::glWindow &_targetWindow) : targetWindow(&_targetWindow) {
        q_3dDraw = qrk::assets::Program("shaders/3d_vertex_shader.vert",
                                        "shaders/3d_fragment_shader.frag");
        q_2dDraw = qrk::assets::Program("shaders/2d_vertex_shader.vert",
                                        "shaders/2d_fragment_shader.frag");
    }

    void QueueDraw(const obj &drawData, DrawTarget target) {
        switch (target) {
            case qrk::DrawTarget::Q_3D_DRAW:
                q_3dObjects.push_back(drawData);
                break;
            case qrk::DrawTarget::Q_2D_DRAW:
                q_2dObjects.push_back(drawData);
                break;
            default:
                break;
        }
    }

    void Draw();

private:
    std::vector<obj> q_3dObjects;
    std::vector<obj> q_2dObjects;

    qrk::glWindow *targetWindow;

    static qrk::assets::Program q_3dDraw;
    static qrk::assets::Program q_2dDraw;
};
}// namespace qrk

#endif// !Q_DRAW
