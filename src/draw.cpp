#include "../include/draw.hpp"

void qrk::Renderer::Draw() {
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
        qrk::mat4 projectionMatrix = qrk::CreatePerspectiveProjectionMatrix(
                qrk::constants::DegToRad(90),
                targetWindow->GetSize().x() / targetWindow->GetSize().y(), 1.f,
                100.f);
    }

    glUseProgram(q_2dDraw.programHandle);
    for (int i = 0; i < q_2dObjects.size(); i++) {}
}