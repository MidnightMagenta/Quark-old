#include "../include/rect.hpp"

qrk::DrawData_2D qrk::Rect::GetDrawData()
{ 
    DrawData_2D data;
    data.VAO = this->VAO;
    data.VBO = this->VBO;
    data.position = this->position;
    data.size = this->size;
    data.color = qrk::ConvertToFloat(this->color);
    data.rotation = this->rotation;
    data.vertexCount = this->vertices.size() / 4;
    return data;
}