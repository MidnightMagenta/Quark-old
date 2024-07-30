#ifndef QRK_OBJECT
#define QRK_OBJECT

#include "../glad/glad.h"
#include "../include/color.hpp"
#include "../include/draw.hpp"
#include "../include/qrk_debug.hpp"
#include "../include/texture.hpp"
#include "../include/vector.hpp"
#include <filesystem>
#include <future>
#include <string>
#include <thread>
#include <vector>

namespace qrk {
class Object {
public:
    Object() = delete;
    explicit Object(const std::string &path, bool async = true)
        : vertexNumber(NULL), loadFinished(false), asyncLoad(async) {
        if (!std::filesystem::exists(path)) {
            qrk::debug::Error("Failed to find file: " + path,
                              qrk::debug::Q_FAILED_TO_FIND_FILE);
        }
        if (async) {
            futureData = promisedData.get_future();
            futureMaterial = promisedMaterial.get_future();
            std::thread loadThread(&LoadObjectAsync,
                                   path, &loadFinished, std::move(promisedData), std::move(promisedMaterial));
            loadThread.detach();
        } else {
            LoadObject(path);
        }
    }

    ~Object() = default;

    bool WaitForLoad(const qrk::glWindow &window) {
        if (asyncLoad) {
            if (!data.empty()) { return false; }
            if (loadFinished) {
                data = futureData.get();
                vertexNumber = static_cast<GLsizei>(data.size()) / 9;
                material = futureMaterial.get();
                return false;
            } else {
                window.GetWindowMessage();
                return true;
            }
        } else
            return false;
    }

    void DeleteData() { std::vector<GLfloat>().swap(data); }

    std::string DumpObjectData(const std::string &path = "logs") const;

    std::vector<GLfloat> data;//vertex texture normals
    qrk::Material material;
    GLsizei vertexNumber;

private:
    static void LoadObjectAsync(const std::string& path, std::atomic_bool *finishedFlag,
                         std::promise<std::vector<GLfloat>> _promisedData,
                         std::promise<qrk::Material> _promisedMaterial);
    void LoadObject(const std::string &path);

    std::atomic_bool loadFinished;
    bool asyncLoad;
    std::promise<std::vector<GLfloat>> promisedData;
    std::future<std::vector<GLfloat>> futureData;

    std::promise<qrk::Material> promisedMaterial;
    std::future<qrk::Material> futureMaterial;

    struct ObjectData {
        std::vector<qrk::vec4f> vertices;
        std::vector<qrk::vec2f> textures;
        std::vector<qrk::vec3f> normals;

        std::vector<int> vertexIndeces;
        std::vector<int> textureIndeces;
        std::vector<int> normalIndeces;

        void Clear() {
            std::vector<qrk::vec4f>().swap(vertices);
            std::vector<qrk::vec3f>().swap(normals);
            std::vector<qrk::vec2f>().swap(textures);

            std::vector<int>().swap(vertexIndeces);
            std::vector<int>().swap(textureIndeces);
            std::vector<int>().swap(normalIndeces);
        }
    };
};

class GLObject {
public:
    GLObject() = delete;
    explicit GLObject(const qrk::Object &_objectData)
        : texture(nullptr), textured(false), VAO(0), VBO(0),
          color({1.f, 1.f, 1.f, 1.f}), position({0, 0, 0}), rotation({0, 0, 0}),
          scale({1, 1, 1}) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, _objectData.data.size() * sizeof(GLfloat),
                     _objectData.data.data(), GL_STATIC_DRAW);
        vertexNumber = _objectData.vertexNumber;
        qrk::mat4 identity = identity4();
        posMatrix = identity;
        rotMatrix = identity;
        sclMatrix = identity;
    }

    explicit GLObject(const std::string &objectPath)
        : texture(nullptr), textured(false), VAO(0),
          VBO(0), color({1.f, 1.f, 1.f, 1.f}), position({0, 0, 0}), rotation({0, 0, 0}), scale({1, 1, 1}) {
        qrk::Object _objectData(objectPath, false);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, _objectData.data.size() * sizeof(GLfloat),
                     _objectData.data.data(), GL_STATIC_DRAW);
        vertexNumber = _objectData.vertexNumber;
        qrk::mat4 identity = identity4();
        posMatrix = identity;
        rotMatrix = identity;
        sclMatrix = identity;
        _objectData.DeleteData();
    }

    void SetPosition(float x, float y, float z) {
        position = qrk::vec3f({x, y, z});
        qrk::mat4 tempMatrix = qrk::CreateTranslationMatrix(x, y, z);
        posMatrix = tempMatrix;
    }
    void SetRotation(float x, float y, float z) {
        rotation = qrk::vec3f({x, y, z});
        qrk::mat4 tempMatrix = qrk::CreateRotationMatrix(x, y, z);
        rotMatrix = tempMatrix;
    }
    void SetScale(float x, float y, float z) {
        scale = qrk::vec3f({x, y, z});
        qrk::mat4 tempMatrix = qrk::CreateScaleMatrix(x, y, z);
        sclMatrix = tempMatrix;
    }
    void SetColor(const qrk::Color &_color) { color = qrk::ConvertToFloat(_color); }
    void SetColor(const qrk::ColorF &_color) { color = _color; }
    void SetTexture(qrk::Texture2D &_texture) {
        texture = &_texture;
        textured = true;
    }
    void RemoveTexture() {
        texture = nullptr;
        textured = false;
    }

    qrk::vec3f GetPosition() { return position; }
    qrk::vec3f GetRotation() { return rotation; }
    qrk::vec3f GetScale() { return scale; }

    qrk::DrawData_3D GetDrawData();

private:
    qrk::Texture2D *texture;
    bool textured;

    GLuint VAO;
    GLuint VBO;
    GLsizei vertexNumber;

    qrk::ColorF color;
    qrk::vec3f position;
    qrk::vec3f rotation;
    qrk::vec3f scale;
    qrk::mat4 posMatrix;
    qrk::mat4 rotMatrix;
    qrk::mat4 sclMatrix;
};
}// namespace qrk

#endif// !QRK_OBJECT