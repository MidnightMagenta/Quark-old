#include "../include/draw.hpp"

void qrk::Renderer::Draw() {
    if (!targetWindow->IsOpen()) { return; }
    if (!targetWindow->IsContextCurrent()) {
        targetWindow->MakeContextCurrent();
    }

    glUseProgram(q_3dDraw.programHandle);

    float fov = 70.f * qrk::units::deg;
    if (this->__settings != nullptr) { fov = this->__settings->fov; }
    qrk::mat4 projectionMatrix = qrk::CreatePerspectiveProjectionMatrix(
            fov,
            (float) targetWindow->GetSize().x() /
                    (float) targetWindow->GetSize().y(),
            1.f, 100.f);

    for (int i = 0; i < q_3dObjects.size(); i++) {
        //generate draw data
        qrk::mat4 transformMatrix = qrk::CreateTranslationMatrix(
                q_3dObjects[i].position.x(), q_3dObjects[i].position.y(),
                q_3dObjects[i].position.z());
        qrk::mat4 rotationMatrix = qrk::CreateRotationMatrix(
                q_3dObjects[i].rotation.x(), q_3dObjects[i].rotation.y(),
                q_3dObjects[i].rotation.z());
        qrk::mat4 scaleMatrix = qrk::CreateScaleMatrix(
                q_3dObjects[i].scale.x(), q_3dObjects[i].scale.y(),
                q_3dObjects[i].scale.z());
        qrk::mat4 identity = qrk::identity4();

        //move the draw data to a unifrom block in the shader
        UBO3D_Data.position = transformMatrix;
        UBO3D_Data.rotation = rotationMatrix;
        UBO3D_Data.scale = scaleMatrix;
        UBO3D_Data.projection = projectionMatrix;
        UBO3D_Data.view = identity;
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
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, UBO3D);

        glBindVertexArray(q_3dObjects[i].VAO);
        glBindBuffer(GL_ARRAY_BUFFER, q_3dObjects[i].VBO);

        //move vertex attibutes to the shader
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

    /*   glUseProgram(q_2dDraw.programHandle);
    for (int i = 0; i < q_2dObjects.size(); i++) {}*/
}