#ifndef Q_DRAW
#define Q_DRAW

#define Q_POINT 0
#define Q_DIRECTIONAL 1

#include "../include/GL_assets.hpp"
#include "../include/matrix.hpp"
#include "../include/texture.hpp"
#include "../include/vector.hpp"
#include "../include/window.hpp"
#include <vector>

namespace qrk {
//draw data types
struct DrawData_3D {
    GLuint VAO = 0;
    GLuint VBO = 0;
    qrk::Texture2D *texture = nullptr;
    bool textured = false;

    GLsizei vertexCount = 0;

    mat4 position = qrk::identity4();
    mat4 rotation = qrk::identity4();
    mat4 scale = qrk::identity4();
    ColorF color = qrk::ColorF(1.f, 1.f, 1.f, 1.f);
};
struct DrawData_2D {
    GLuint VAO = 0;
    GLuint VBO = 0;
    qrk::vec2f position = qrk::vec2f({0, 0});
    qrk::vec2f size = qrk::vec2f({100, 100});
    qrk::ColorF color = {1.f, 1.f, 1.f, 1.f};
    float zLayer = 0.f;
    qrk::Texture2D *texture = nullptr;
    float rotation = 0.f;
    GLsizei vertexCount = 0;
};
struct DrawData_Text {
    GLuint VAO = 0;
    GLuint VBO = 0;
    float zLayer = 0.f;
    qrk::ColorF color = {1.f, 1.f, 1.f, 1.f};
    qrk::Texture2D *texture = nullptr;
    GLsizei vertexCount = 0;
};
template<typename T>
concept drawDataStruct =
        std::is_same_v<T, DrawData_3D> || std::is_same_v<T, DrawData_2D> ||
        std::is_same_v<T, DrawData_Text>;

struct Material {
    float shininess = 25.f;
    char padding[12];
    vec3f specular = qrk::vec3f({0.5f, 0.5f, 0.5f});
    vec3f diffuse = qrk::vec3f({0.8f, 0.8f, 0.8f});
    vec3f ambient = qrk::vec3f({1.f, 1.f, 1.f});
};
struct UniformData3D {
    qrk::mat4 position = identity4();
    qrk::mat4 rotation = identity4();
    qrk::mat4 scale = identity4();
    qrk::mat4 view = identity4();
    qrk::mat4 projection = identity4();
    qrk::vec4f color = qrk::vec4f({1, 1, 1, 1});
    qrk::vec4f cameraPosition = qrk::vec4f({0, 0, 0, 0});
    Material material;
};
struct UniformData2D {
    qrk::mat4 rotation = identity4();
    qrk::vec2f position = qrk::vec2f({0, 0});
    qrk::vec2f size = qrk::vec2f({1, 1});
    qrk::vec4f color = qrk::vec4f({1, 1, 1, 1});
    float zLayer = 0.f;
};
struct UniformDataText {
    qrk::vec4f color = qrk::vec4f({1, 1, 1, 1});
    qrk::vec2f screenSize = qrk::vec2f({0, 0});
    float zLayer = 0.f;
    char padding[4];
};
struct LightSource {
    LightSource() {
        position = qrk::vec3f({0.f, 0.f, 0.f});
        lightType = Q_POINT;

        constant = 0.f;
        linear = 1.f;
        quadratic = 0.f;

        ambient = qrk::vec3f({0.1f, 0.1f, 0.1f});
        diffuse = qrk::vec3f({1.f, 1.f, 1.f});
        specular = qrk::vec3f({0.3f, 0.3f, 0.3f});
    }
    LightSource(const qrk::vec3f &_position, const qrk::Color &color) {
        position = _position;
        lightType = Q_POINT;

        constant = 0.f;
        linear = 1.f;
        quadratic = 0.f;

        qrk::ColorF cf = qrk::ConvertToFloat(color);
        ambient = qrk::vec3f({cf.r * 0.1f, cf.g * 0.1f, cf.b * 0.1f});
        diffuse = qrk::vec3f({cf.r, cf.g, cf.b});
        specular = qrk::vec3f({cf.r * 0.3f, cf.g * 0.3f, cf.b * 0.3f});
    }

    int lightType;
    float constant;
    float linear;
    float quadratic;

    vec3f position;

    vec3f ambient;
    vec3f diffuse;
    vec3f specular;
};

struct RendererSettings {
    bool depthTest = true;
    bool cullFaces = true;
    bool alpha = true;
    bool multisample = true;
};

class qb_GL_Renderer {
public:
    qb_GL_Renderer() = delete;
    qb_GL_Renderer(qrk::glWindow &_targetWindow,
                   qrk::RendererSettings _settings = {true, true, true, true});
    ~qb_GL_Renderer() {}

    template<drawDataStruct draw_t>
    void QueueDraw(const draw_t &drawData, bool UI = false) {
        if constexpr (std::is_same_v<draw_t, DrawData_3D>) {
            Queue3dDraw(drawData);
        } else if constexpr (std::is_same_v<draw_t, DrawData_2D>) {
            if (UI) {
                QueueUIDraw(drawData);
            } else {
                Queue2dDraw(drawData);
            }
        } else if constexpr (std::is_same_v<draw_t, DrawData_Text>) {
            QueueTextDraw(drawData);
        } else {
            qrk::debug::Error("Invalid draw call", qrk::debug::Q_INVALID_DRAW);
        }
    }

    void AddLightSource(const qrk::LightSource &lightSource) {
        q_3dLightSources.push_back(lightSource);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightSource_SSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER,
                     q_3dLightSources.size() * sizeof(LightSource),
                     q_3dLightSources.data(), GL_DYNAMIC_COPY);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
    void RemoveLightSource(size_t index) {
        q_3dLightSources.erase(q_3dLightSources.begin() + index);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightSource_SSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER,
                     q_3dLightSources.size() * sizeof(LightSource),
                     q_3dLightSources.data(), GL_DYNAMIC_COPY);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void Draw();

private:
    //vectors containing draw queue
    std::vector<DrawData_3D> q_3dObjects;
    std::vector<DrawData_2D> q_2dObjects;
    std::vector<DrawData_2D> q_UIObjects;
    std::vector<DrawData_Text> q_Text;
    std::vector<LightSource> q_3dLightSources;


    //3d draw program and associated 3d draw specific uniform locations
    qrk::assets::Program q_3dDraw;
    qrk::UniformData3D UBO3D_Data;
    GLuint UBO3D;
    GLuint textureID_3d;
    GLuint texturedID_3d;
    GLuint lightSource_SSBO;

    //2d draw program and associated 2d draw specific uniform locations;
    qrk::assets::Program q_2dDraw;
    qrk::UniformData2D UBO2D_Data;
    GLuint UBO2D;
    GLuint textureID_2d;
    GLuint texturedID_2d;
    //text draw program and associated uniform locations
    qrk::assets::Program q_textDraw;
    qrk::UniformDataText UBO_Text_data;
    GLuint UBO_Text;
    GLuint textureID_Text;

    //misc variables
    qrk::glWindow *targetWindow;

    void Queue3dDraw(const DrawData_3D &drawData) {
        q_3dObjects.push_back(drawData);
    }
    void Queue2dDraw(const DrawData_2D &drawData) {
        q_2dObjects.push_back(drawData);
    }
    void QueueUIDraw(const DrawData_2D &drawData) {
        q_UIObjects.push_back(drawData);
    }
    void QueueTextDraw(const DrawData_Text &drawData) {
        q_Text.push_back(drawData);
    }
};
}// namespace qrk

#endif// !Q_DRAW