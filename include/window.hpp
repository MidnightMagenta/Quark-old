#ifndef QRK_WINDOW
#define QRK_WINDOW

#include "../dependencies/OpenGL.h"
#include "../include/color.hpp"
#include "../include/qrk_debug.hpp"
#include "../include/vector.hpp"
#include <Windows.h>
#include <iostream>
#include <string>

namespace qrk {
class GLTools {
public:
    //context creation tools
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
};

class glWindow : GLTools {
public:
    glWindow(const std::string &windowName, qrk::vec2u size, int windowStyle,
             qrk::Color resizeColor = {255, 255, 255, 255})
        : Open(true), windowSize(size) {
        Create(windowName, size, windowStyle, resizeColor);
    }

    bool Create(const std::string &windowName, qrk::vec2u size, int windowStyle,
                qrk::Color resizeColor = {255, 255, 255, 255});
    bool CreateContext();

    void SwapWindowBuffers() { SwapBuffers(deviceContext); }
    void MakeContextCurrent() { wglMakeCurrent(deviceContext, glContext); }
    void Clear(qrk::Color color) {
        qrk::ColorF fColor = qrk::ConvertToFloat(color);
        glClearColor(fColor.r, fColor.g, fColor.b, fColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    bool IsOpen() { return Open; }
    void GetWindowMessage() {
        MSG msg = {};
        if (PeekMessage(&msg, this->window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    //get and set window parameters
    qrk::vec2u GetSize() {
        RECT windowRect;
        if (!GetWindowRect(window, &windowRect)) {
            std::string error = "Could not retrieve window size information: " +
                                std::to_string(GetLastError());
            qrk::Debug::ShowErrorBox(error);
            qrk::Debug::LogError(error);
            return qrk::vec2u({0, 0});
        }
        return qrk::vec2u(
                {(unsigned int) (windowRect.right - windowRect.left),
                 (unsigned int) (windowRect.bottom - windowRect.top)});
    }
    void SetSize(qrk::vec2u newSize) {
        if (!SetWindowPos(window, HWND_TOP, 0, 0, newSize.x(), newSize.y(),
                          SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOACTIVATE)) {
            std::string error = std::to_string(GetLastError());
            qrk::Debug::ShowErrorBox(error);
            qrk::Debug::LogError(error);
        }
    }

    qrk::vec2u GetPosition() {
        RECT windowRect;
        if (!GetWindowRect(window, &windowRect)) {
            std::string error = "Could not retrieve window size information: " +
                                std::to_string(GetLastError());
            qrk::Debug::ShowErrorBox(error);
            qrk::Debug::LogError(error);
            return qrk::vec2u({0, 0});
        }
        return qrk::vec2u({(unsigned int) (windowRect.left),
                           (unsigned int) (windowRect.top)});
    }
    void SetPosition(qrk::vec2u newPos) {
        if (!SetWindowPos(window, HWND_TOP, 0, 0, newPos.x(), newPos.y(),
                          SWP_NOSIZE | SWP_NOREPOSITION | SWP_NOACTIVATE)) {
            std::string error = std::to_string(GetLastError());
            qrk::Debug::ShowErrorBox(error);
            qrk::Debug::LogError(error);
        }
    }

protected:
    WNDCLASSEX windowClass;
    HWND window;
    HDC deviceContext;
    HGLRC glContext;

    qrk::vec2u windowSize;
    bool Open;

    static LRESULT CALLBACK Process(HWND hwnd, UINT uMsg, WPARAM wParam,
                                    LPARAM lParam);
    static LRESULT CALLBACK DummyProcess(HWND hwnd, UINT uMsg, WPARAM wParam,
                                         LPARAM lParam) {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }//dummy process for context creations

    LRESULT WndProcess(UINT message, WPARAM wParam, LPARAM lParam);

    void LoadContextCreationTools();
};
}// namespace qrk

#endif// !QRK_WINDOW