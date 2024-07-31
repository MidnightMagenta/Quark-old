#ifndef Q_EVENT
#define Q_EVENT

#include "../include/window.hpp"

namespace qrk {
enum KEY {
    A = 0x41,
    B = 0x42,
    C = 0x43,
    D = 0x44,
    E = 0x45,
    F = 0x46,
    G = 0x47,
    H = 0x48,
    I = 0x49,
    J = 0x4A,
    K = 0x4B,
    L = 0x4C,
    M = 0x4D,
    N = 0x4E,
    O = 0x4F,
    P = 0x50,
    Q = 0x51,
    R = 0x52,
    S = 0x53,
    T = 0x54,
    U = 0x55,
    V = 0x56,
    W = 0x57,
    X = 0x58,
    Y = 0x59,
    Z = 0x5A,
    NUM_1 = 0x30,
    NUM_2 = 0x31,
    NUM_3 = 0x32,
    NUM_4 = 0x33,
    NUM_5 = 0x34,
    NUM_6 = 0x35,
    NUM_7 = 0x36,
    NUM_8 = 0x37,
    NUM_9 = 0x38,
    NUM_0 = 0x39,
    F1 = 0x70,
    F2 = 0x71,
    F3 = 0x72,
    F4 = 0x73,
    F5 = 0x74,
    F6 = 0x75,
    F7 = 0x76,
    F8 = 0x77,
    F9 = 0x78,
    F10 = 0x79,
    F11 = 0x7A,
    F12 = 0x7B,
    CAPSLOCK,
    SPACE = 0x20,
    TAB = 0x09,
    ENTER = 0x0D,
    ESCAPE = 0x1B,
    BACKSPACE = 0x08,
    DEL = 0x2E,
    INSERT = 0x2D,
    HOME = 0x24,
    END = 0x23,
    PG_UP = 0x21,
    PG_DOWN = 0x22,
    UP = 0x26,
    DOWN = 0x28,
    LEFT = 0x25,
    RIGHT = 0x27,
    NUMPAD_1 = 0x60,
    NUMPAD_2 = 0x61,
    NUMPAD_3 = 0x62,
    NUMPAD_4 = 0x63,
    NUMPAD_5 = 0x64,
    NUMPAD_6 = 0x65,
    NUMPAD_7 = 0x66,
    NUMPAD_8 = 0x67,
    NUMPAD_9 = 0x68,
    NUMPAD_0 = 0x69,
    NUMPAD_DOT = 0x6E,
    NUMPAD_MUL = 0x6A,
    NUMPAD_DIV = 0x6F,
    NUMPAD_ADD = 0x6B,
    NUMPAD_SUB = 0x6D,
    NUM_LOCK = 0x90,
    L_WIN = 0x5B,
    R_WIN = 0x5C,
    L_CTRL = 0xA2,
    R_CTRL = 0xA3,
    L_ALT = 0xA4,
    R_ALT = 0xA5,
    L_SHIFT = 0xA0,
    R_SHIFT = 0xA1,
    SHIFT = 0x10,
    CTRL = 0x11,
    ALT = 0x12
};

enum MOUSE {
    L_BUTTON = 0x01,
    R_BUTTON = 0x02,
    M_BUTTON = 0x04,
    X1_BUTTON = 0x05,
    X2_BUTTON = 0x06
};

class Event {
public:
    Event() = delete;
    Event(qrk::glWindow &window)
        : targetWindow(&window), lastMessage(window.GetWindowMessage()),
          mousePosition({0, 0}) {}
    void UpdateWindow() { 
        lastMessage = targetWindow->GetWindowMessage(); }
    /// <summary>
    /// Check if key is down
    /// </summary>
    bool KeyDown(qrk::KEY key);
    /// <summary>
    /// Check if key is up
    /// </summary>
    bool KeyUp(qrk::KEY key);
    ///// <summary>
    ///// Return true on the frame they key was pushed down
    ///// </summary>
    //bool OnKeyDown(qrk::KEY key);
    ///// <summary>
    ///// Return true on the frame the key was released
    ///// </summary>
    //bool OnKeyUp(qrk::KEY key);
    /// <summary>
    /// Get mouse position in relative coordinates while the cursor is in the client area (0, 0 is top left corner of the window)
    /// </summary>
    qrk::vec2i GetMousePosition();
    /// <summary>
    /// Get mouse position in absolute coordinates (0, 0 is top left corner of the primary display)
    /// </summary>
    qrk::vec2i GetAbsoluteMousePosition();
    /// <summary>
    /// Get mouse position in relative coordinates (0, 0 is top left corner of the window)
    /// </summary>
    qrk::vec2i GetRelativeMousePosition();

private:
    qrk::glWindow *targetWindow;
    MSG lastMessage;

    qrk::vec2i mousePosition;
};
}// namespace qrk

#endif// !Q_EVENT
