#include "../include/event.hpp"

bool qrk::Event::KeyPressed(qrk::KEY key) {
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}