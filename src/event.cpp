#include "../include/event.hpp"

bool qrk::Event::KeyPressed(qrk::KEY key) {
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

qrk::vec2i qrk::Event::GetMousePosition() {
    if (this->lastMessage.message == WM_MOUSEMOVE) {
        int xPos = GET_X_LPARAM(this->lastMessage.lParam);
        int yPos = GET_Y_LPARAM(this->lastMessage.lParam);
        this->mousePosition = qrk::vec2i({xPos, yPos});
    }
    return this->mousePosition;
}