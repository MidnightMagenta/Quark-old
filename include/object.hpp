#ifndef QRK_OBJECT
#define QRK_OBJECT

#include "../glad/glad.h"
#include "../include/GL_assets.hpp"
#include "../include/color.hpp"
#include "../include/matrix.hpp"
#include "../include/qrk_debug.hpp"
#include "../include/vector.hpp"
#include <Windows.h>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <future>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace qrk {
class Object {
public:
    Object() = delete;
    Object(const std::string &path, bool async = true)
        : loadFinished(false), asyncLoad(async) {
        if (async) {
            _path = path;
            futureData = promisedData.get_future();
            std::thread loadThread(std::bind(&qrk::Object::LoadObjectAsync,
                                             this, std::placeholders::_1,
                                             std::placeholders::_2),
                                   path, std::move(promisedData));
            loadThread.detach();
        } else {
            LoadObject(path);
        }
    }

    ~Object() {}

    bool WaitForLoad() {
        if (asyncLoad) {
            if (loadFinished) {
                data = futureData.get();
                return false;
            } else
                return true;
        } else
            return false;
    }

    std::string DumpObjectData(std::string path = "logs");

    std::vector<GLfloat> data;//vertex texture normals
private:
    void LoadObjectAsync(std::string path,
                         std::promise<std::vector<GLfloat>> _promisedData);
    void LoadObject(const std::string &path);

    std::string _path;
    std::atomic_bool loadFinished;
    bool asyncLoad;
    std::promise<std::vector<GLfloat>> promisedData;
    std::future<std::vector<GLfloat>> futureData;

    struct ObjectData {
        std::vector<qrk::vec4f> vertices;
        std::vector<qrk::vec2f> textures;
        std::vector<qrk::vec3f> normals;

        std::vector<int> vertexIndeces;
        std::vector<int> textureIndeces;
        std::vector<int> normalIndeces;

        void Clear() {
            vertices.clear();
            vertices.shrink_to_fit();
            textures.clear();
            textures.shrink_to_fit();
            normals.clear();
            normals.shrink_to_fit();
            vertexIndeces.clear();
            vertexIndeces.shrink_to_fit();
            textureIndeces.clear();
            textureIndeces.shrink_to_fit();
            normalIndeces.clear();
            normalIndeces.shrink_to_fit();
        }
    };
};

class GLObject {
public:
    GLObject() = delete;
    GLObject(qrk::Object &_objectData, qrk::assets::Program *_program)
        : objectData(&_objectData), position({0, 0, 0}), rotation({0, 0, 0}),
          scale({0, 0, 0}), color({1.f, 1.f, 1.f, 1.f}) {
        //if (!program->bound) glUseProgram(program->programHandle);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, objectData->data.size() * sizeof(GLfloat),
                     objectData->data.data(), GL_STATIC_DRAW);
    }

    void SetPosition(float x, float y, float z) {
        position = qrk::vec3f({x, y, z});
    }
    void SetRotation(float x, float y, float z) {
        rotation = qrk::vec3f({x, y, z});
    }
    void SetScale(float x, float y, float z) { scale = qrk::vec3f({x, y, z}); }
    void SetColor(qrk::Color _color) { color = qrk::ConvertToFloat(_color); }
    void SetColor(qrk::ColorF _color) { color = _color; }

    qrk::vec3f GetPosition() { return position; }
    qrk::vec3f GetRotation() { return rotation; }
    qrk::vec3f GetScale() { return scale; }

private:
    qrk::Object *objectData;

    GLuint VAO;
    GLuint VBO;

    qrk::vec3f position;
    qrk::vec3f rotation;
    qrk::vec3f scale;
    qrk::ColorF color;
};
}// namespace qrk

#endif// !QRK_OBJECT