#include "../include/rect.hpp"

qrk::DrawData_2D qrk::Rect::GetDrawData() {
    DrawData_2D data;
    data.VAO = this->VAO;
    data.VBO = this->VBO;
    data.position = qrk::vec2f(
            {this->position.x() - this->originOffset.x(),
             this->position.y() - this->originOffset.y()});
    data.size = this->size;
    data.color = qrk::ConvertToFloat(this->color);
    data.rotation = this->rotation;
    data.vertexCount = (GLsizei) vertices.size() / 4;
    return data;
}