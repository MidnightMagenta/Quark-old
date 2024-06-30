#ifndef QRK_WINDOW
#define QRK_WINDOW

#include "../dependencies/OpenGL.h"
#include "../include/color.hpp"
#include "../include/qrk_debug.hpp"
#include "../include/vector.hpp"
#include <Windows.h>
#include <iostream>
#include <string>
#include <windowsx.h>

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
        : Open(true), windowSize(size), mouseMovedCallback(nullptr) {
        Create(windowName, size, windowStyle, multisamplingLevel, _clearColor);
    }
    ~glWindow() {}

    bool Create(const std::string &windowName, qrk::vec2u size,
                int windowStyle = Q_WINDOW_DEFAULT, int multisamplingLevel = 8,
                qrk::Color _clearColor = {255, 255, 255, 255});
    void Close() {
        wglDeleteContext(this->glContext);
        DestroyWindow(this->window);
        this->Open = false;
    }
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
    void SetClearColor(qrk::Color _clearColor) {
        qrk::ColorF fColor = qrk::ConvertToFloat(_clearColor);
        glClearColor(fColor.r, fColor.g, fColor.b, fColor.a);
    }

    void SetSwapInterval(int interval) { wglSwapIntervalEXT(interval); }

    bool IsOpen() { return Open; }
    MSG GetWindowMessage() {
        MSG msg = {};
        if (PeekMessage(&msg, this->window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return msg;
    }

    //get and set window parameters
    void q_HideCursor() { ShowCursor(FALSE); }
    void q_ShowCursor() { ShowCursor(TRUE); }
    qrk::vec2u GetSize() {
        RECT windowRect;
        if (!GetWindowRect(window, &windowRect)) {
            std::string error = "Could not retrieve window size information. Error code: " +
                                std::to_string(GetLastError());
            qrk::debug::ShowErrorBox(error);
            qrk::debug::LogError(error);
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
            qrk::debug::ShowErrorBox(error);
            qrk::debug::LogError(error);
        }
    }

    qrk::vec2u GetPosition() {
        RECT windowRect;
        if (!GetWindowRect(window, &windowRect)) {
            std::string error = "Could not retrieve window size information: " +
                                std::to_string(GetLastError());
            qrk::debug::ShowErrorBox(error);
            qrk::debug::LogError(error);
            return qrk::vec2u({0, 0});
        }
        return qrk::vec2u({(unsigned int) (windowRect.left),
                           (unsigned int) (windowRect.top)});
    }
    void SetPosition(qrk::vec2u newPos) {
        if (!SetWindowPos(window, HWND_TOP, 0, 0, newPos.x(), newPos.y(),
                          SWP_NOSIZE | SWP_NOREPOSITION | SWP_NOACTIVATE)) {
            std::string error = std::to_string(GetLastError());
            qrk::debug::ShowErrorBox(error);
            qrk::debug::LogError(error);
        }
    }
    HWND GetNativeWindowHandle() { return this->window; }
    HDC GetNativeDeviceContextHandle() { return this->deviceContext; }
    HGLRC GetNativeContextHandle() { return this->glContext; }

    void SetMouseMovedCallback(void (*callback)(qrk::vec2i mousePosition)) {
        this->mouseMovedCallback = callback;
    }

protected:
    WNDCLASSEX windowClass;
    HWND window;
    HDC deviceContext;
    HGLRC glContext;

    qrk::vec2u windowSize;
    bool Open;

    //callback functions
    void (*mouseMovedCallback)(qrk::vec2i mousePosition);

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