#include "../include/draw.hpp"

qrk::qb_GL_Renderer::qb_GL_Renderer(qrk::glWindow &_targetWindow,
                                    qrk::settings *_settings)
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
    //compile the 3d program
    q_3dDraw =
            qrk::assets::Program("resources/shaders/3d_vertex_shader.vert",
                                 "resources/shaders/3d_fragment_shader.frag");
    textureID = glGetUniformLocation(q_3dDraw.programHandle, "inTexture");
    texturedID = glGetUniformLocation(q_3dDraw.programHandle, "textured");
    //create the 3d UBO
    glGenBuffers(1, &UBO3D);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO3D);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformData3D), &UBO3D_Data,
                 GL_DYNAMIC_COPY);
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, UBO3D);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

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
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    //compile the 2d program
    q_2dDraw =
            qrk::assets::Program("resources/shaders/2d_vertex_shader.vert",
                                 "resources/shaders/2d_fragment_shader.frag");
    glGenBuffers(1, &UBO2D);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO2D);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformData2D), &UBO2D_Data,
                 GL_DYNAMIC_COPY);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, UBO2D);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void qrk::qb_GL_Renderer::Draw() {
    if (!targetWindow->IsOpen()) { return; }
    if (!targetWindow->IsContextCurrent()) {
        targetWindow->MakeContextCurrent();
    }
    //3d draw
    this->q_3dDraw.UseProgram();

    float fov = 70.f * qrk::units::deg;
    if (this->__settings != nullptr) { fov = this->__settings->fov; }
    qrk::vec2u screenSize = targetWindow->GetSize();
    qrk::mat4 projectionMatrix = qrk::CreatePerspectiveProjectionMatrix(
            fov, (float) screenSize.x() / (float) screenSize.y(), 1.f, 100.f);
    UBO3D_Data.projection = projectionMatrix;
    qrk::mat4 identity = qrk::identity4();
    UBO3D_Data.view = identity;

    for (int i = 0; i < q_3dObjects.size(); i++) {
        //move the draw data to a unifrom block in the shader
        UBO3D_Data.position = q_3dObjects[i].position;
        UBO3D_Data.rotation = q_3dObjects[i].rotation;
        UBO3D_Data.scale = q_3dObjects[i].scale;
        UBO3D_Data.color =
                qrk::vec4f({q_3dObjects[i].color.r, q_3dObjects[i].color.g,
                            q_3dObjects[i].color.b, q_3dObjects[i].color.a});
        if (q_3dObjects[i].textured && q_3dObjects[i].texture != nullptr) {
            glUniform1i(texturedID, GL_TRUE);
            q_3dObjects[i].texture->BindTexture();
            glUniform1i(textureID, 0);
        } else {
            glUniform1i(texturedID, GL_FALSE);
        }

        glBindBuffer(GL_UNIFORM_BUFFER, UBO3D);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(UniformData3D),
                        &UBO3D_Data);
        glUniformBlockBinding(q_3dDraw.programHandle,
                              q_3dDraw.uniformBlockIndex, 3);

        glBindVertexArray(q_3dObjects[i].VAO);
        glBindBuffer(GL_ARRAY_BUFFER, q_3dObjects[i].VBO);

        //generate vertex attributes
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                              (void *) 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                              (void *) (4 * sizeof(GLfloat)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                              (void *) (6 * sizeof(GLfloat)));

        //draw
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glDrawArrays(GL_TRIANGLES, 0, q_3dObjects[i].vertexCount);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }
    q_3dObjects.clear();

    //2d draw
    this->q_2dDraw.UseProgram();

    for (int i = 0; i < q_2dObjects.size(); i++) {
        qrk::mat4 posMatrix = CreateTranslationMatrix(
                (q_2dObjects[i].position.x()) /
                        screenSize.x(),
                -(q_2dObjects[i].position.y()) /
                        screenSize.y(),
                0.f);
        UBO2D_Data.position = posMatrix;
        qrk::mat4 sizeMatrix = CreateScaleMatrix(
                q_2dObjects[i].size.x() / screenSize.x(),
                q_2dObjects[i].size.y() / screenSize.y(), 0.f);
        UBO2D_Data.size = sizeMatrix;
        qrk::mat4 rotMatrix =
                qrk::CreateRotationMatrix(q_2dObjects[i].rotation, 0.f, 0.f);
        UBO2D_Data.rotation = rotMatrix;
        UBO2D_Data.color =
                qrk::vec4f({q_2dObjects[i].color.r, q_2dObjects[i].color.b,
                            q_2dObjects[i].color.g, q_2dObjects[i].color.a});
        glBindBuffer(GL_UNIFORM_BUFFER, UBO2D);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(UniformData2D),
                        &UBO2D_Data);
        glUniformBlockBinding(q_2dDraw.programHandle,
                              q_2dDraw.uniformBlockIndex, 2);

        glBindVertexArray(q_2dObjects[i].VAO);
        glBindBuffer(GL_ARRAY_BUFFER, q_2dObjects[i].VBO);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                              (void *) 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                              (void *) (2 * sizeof(GLfloat)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glDrawArrays(GL_TRIANGLES, 0, q_2dObjects[i].vertexCount);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }
    q_2dObjects.clear();
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    qrk::UnbindProgram();
}