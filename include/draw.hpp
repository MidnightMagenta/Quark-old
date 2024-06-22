#ifndef Q_DRAW
#define Q_DRAW

#include "../include/GL_assets.hpp"
#include "../include/constants.hpp"
#include "../include/matrix.hpp"
#include "../include/texture.hpp"
#include "../include/vector.hpp"
#include "../include/window.hpp"
#include "glad/glad.h"
#include <vector>

namespace qrk {
enum class DrawTarget { Q_3D_DRAW, Q_2D_DRAW };

struct obj {
    GLuint VAO = 0;
    GLuint VBO = 0;
    qrk::Texture2D *texture = nullptr;
    bool textured = false;

    GLsizei vertexCount = 0;

    vec3f position = qrk::vec3f({0.f, 0.f, 0.f});
    vec3f rotation = qrk::vec3f({0.f, 0.f, 0.f});
    vec3f scale = qrk::vec3f({1.f, 1.f, 1.f});
    ColorF color = qrk::ColorF(1.f, 1.f, 1.f, 1.f);
};

struct UniformData3D {
    qrk::mat4 position = identity4();
    qrk::mat4 rotation = identity4();
    qrk::mat4 scale = identity4();
    qrk::mat4 view = identity4();
    qrk::mat4 projection = identity4();
    qrk::vec4f color = qrk::vec4f({1, 1, 1, 1});
};
struct LightSource {
public:
    qrk::vec3f lightSourcePosition;
private:
    float idiot;
public:
    qrk::vec4f lightSourceColor;
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
        textureID = glGetUniformLocation(q_3dDraw.programHandle, "inTexture");
        texturedID = glGetUniformLocation(q_3dDraw.programHandle, "textured");
        //create the 3d UBO
        glGenBuffers(1, &UBO3D);
        glBindBuffer(GL_UNIFORM_BUFFER, UBO3D);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformData3D), &UBO3D_Data,
                     GL_DYNAMIC_COPY);
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, UBO3D);

        //create the light source SSBO
        glGenBuffers(1, &lightSource_SSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightSource_SSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER,
                     q_3dLightSources.size() * sizeof(LightSource), nullptr,
                     GL_DYNAMIC_COPY);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
                        q_3dLightSources.size() * sizeof(LightSource),
                        q_3dLightSources.data());
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, lightSource_SSBO);
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
    void AddPointLightSource(const qrk::LightSource &lightSource) {
        q_3dLightSources.push_back(lightSource);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightSource_SSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER,
                     q_3dLightSources.size() * sizeof(LightSource),
                     q_3dLightSources.data(), GL_DYNAMIC_COPY);
    }
    void RemovePointLightSource(size_t index) {
        q_3dLightSources.erase(q_3dLightSources.begin() + index);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightSource_SSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER,
                     q_3dLightSources.size() * sizeof(LightSource),
                     q_3dLightSources.data(), GL_DYNAMIC_COPY);
    }

    void Draw();

private:
    //vectors containing draw queue
    std::vector<obj> q_3dObjects;
    std::vector<LightSource> q_3dLightSources;

    //3d draw program, and associated 3d draw specific uniform locations
    qrk::assets::Program q_3dDraw;
    qrk::UniformData3D UBO3D_Data;
    GLuint UBO3D;
    GLuint textureID;
    GLuint texturedID;
    GLuint lightSource_SSBO;

    //misc variables
    qrk::glWindow *targetWindow;
    qrk::settings *__settings;
};
}// namespace qrk

#endif// !Q_DRAW