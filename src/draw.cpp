#include "../include/draw.hpp"

void qrk::Renderer::Draw() {
    if (!targetWindow->IsOpen()) { return; }

    glUseProgram(q_3dDraw.programHandle);

    for (int i = 0; i < q_3dObjects.size(); i++) {
        glBindVertexArray(q_3dObjects[i].VAO);
        glBindBuffer(GL_ARRAY_BUFFER, q_3dObjects[i].VBO);

        qrk::mat4 transformMatrix = qrk::CreateTranslationMatrix(
                q_3dObjects[i].position.x(), q_3dObjects[i].position.y(),
                q_3dObjects[i].position.z());
        qrk::mat4 rotationMatrix = qrk::CreateRotationMatrix(
                q_3dObjects[i].rotation.x(), q_3dObjects[i].rotation.y(),
                q_3dObjects[i].rotation.z());
        qrk::mat4 scaleMatrix = qrk::CreateScaleMatrix(
                q_3dObjects[i].scale.x(), q_3dObjects[i].scale.y(),
                q_3dObjects[i].scale.z());
        float fov = 70.f * qrk::units::deg;
        if (this->__settings != nullptr) { fov = this->__settings->fov; }
        qrk::mat4 projectionMatrix = qrk::CreatePerspectiveProjectionMatrix(
                fov,
                (float) targetWindow->GetSize().x() /
                        (float) targetWindow->GetSize().y(),
                1.f, 100.f);
        qrk::mat4 identity({1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});

        //add texture binding here once texture class done

        glUniformMatrix4fv(
                glGetUniformLocation(q_3dDraw.programHandle, "posMatrix"), 1,
                GL_TRUE, &transformMatrix.data[0][0]);
        glUniformMatrix4fv(
                glGetUniformLocation(q_3dDraw.programHandle, "rotMatrix"), 1,
                GL_TRUE, &rotationMatrix.data[0][0]);
        glUniformMatrix4fv(
                glGetUniformLocation(q_3dDraw.programHandle, "scaleMatrix"), 1,
                GL_TRUE, &scaleMatrix.data[0][0]);
        glUniformMatrix4fv(
                glGetUniformLocation(q_3dDraw.programHandle, "projMatrix"), 1,
                GL_TRUE, &projectionMatrix.data[0][0]);
        glUniformMatrix4fv(
                glGetUniformLocation(q_3dDraw.programHandle, "viewMatrix"), 1,
                GL_TRUE, &identity.data[0][0]);

        glUniform4f(glGetUniformLocation(q_3dDraw.programHandle, "color"),
                    q_3dObjects[i].color.r, q_3dObjects[i].color.g,
                    q_3dObjects[i].color.b, q_3dObjects[i].color.a);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                              (void *) 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                              (void *) (4 * sizeof(GLfloat)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                              (void *) (6 * sizeof(GLfloat)));

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