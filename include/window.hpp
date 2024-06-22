#ifndef QRK_WINDOW
#define QRK_WINDOW

#include "../dependencies/OpenGL.h"
#include "../include/color.hpp"
#include "../include/qrk_debug.hpp"
#include "../include/vector.hpp"
#include <Windows.h>
#include <iostream>
#include <string>

#define Q_WINDOW_DEFAULT WS_OVERLAPPEDWINDOW
#define Q_WINDOW_NONRESIZABLE WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU

namespace qrk {
class GLTools {
public:
    //context creation tools
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;
};

class glWindow : GLTools {
public:
    glWindow(const std::string &windowName, qrk::vec2u size, int windowStyle,
             int multisamplingLevel,
             qrk::Color _clearColor = {255, 255, 255, 255})
        : Open(true), windowSize(size) {
        Create(windowName, size, windowStyle, multisamplingLevel, _clearColor);
    }
    ~glWindow() {}

    bool Create(const std::string &windowName, qrk::vec2u size,
                int windowStyle = Q_WINDOW_DEFAULT, int multisamplingLevel = 8,
                qrk::Color _clearColor = {255, 255, 255, 255});
    void Close() { PostQuitMessage(0); }
    void Activate() { SetActiveWindow(window); }
    bool CreateContext(int multisamplingLevel);

    void SwapWindowBuffers() { SwapBuffers(deviceContext); }
    bool IsContextCurrent() {
        if (wglGetCurrentContext() == glContext) {
            return true;
        } else {
            return false;
        }
    }
    void MakeContextCurrent() { wglMakeCurrent(deviceContext, glContext); }
    void Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
    void ChangeClearColor(qrk::Color _clearColor) {
        qrk::ColorF fColor = qrk::ConvertToFloat(_clearColor);
        glClearColor(fColor.r, fColor.g, fColor.b, fColor.a);
    }

    void SetSwapInterval(int interval) { wglSwapIntervalEXT(interval); }

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