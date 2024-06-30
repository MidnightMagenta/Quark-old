#include "../include/event.hpp"

bool qrk::Event::KeyPressed(qrk::KEY key) {
    if (!targetWindow->IsOpen()) { return false; }
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

qrk::vec2i qrk::Event::GetMousePosition() {
    if (!targetWindow->IsOpen()) { return qrk::vec2i({0, 0}); }
    if (this->lastMessage.message == WM_MOUSEMOVE) {
        int xPos = GET_X_LPARAM(this->lastMessage.lParam);
        int yPos = GET_Y_LPARAM(this->lastMessage.lParam);
        this->mousePosition = qrk::vec2i({xPos, yPos});
    }
    return this->mousePosition;
}

qrk::vec2i qrk::Event::GetAbsoluteMousePosition() {
    if (!targetWindow->IsOpen()) { return qrk::vec2i({0, 0}); }
    POINT p;
    if (GetCursorPos(&p)) {
        return qrk::vec2i({p.x, p.y});
    } else {
        qrk::debug::Warning("Could not get mouse position");
        return qrk::vec2i({0, 0});
    }
}

qrk::vec2i qrk::Event::GetRelativeMousePosition() {
    if (!targetWindow->IsOpen()) { return qrk::vec2i({0, 0}); }
    POINT p;
    if (GetCursorPos(&p)) {
        if (!ScreenToClient(targetWindow->GetNativeWindowHandle(), &p)) {
            qrk::debug::Warning(
                    "Could not convert mouse position to relative coordinates");
            return qrk::vec2i({0, 0});
        }
        return qrk::vec2i({p.x, p.y});
    } else {
        qrk::debug::Warning("Could not get mouse position");
        return qrk::vec2i({0, 0});
    }
}