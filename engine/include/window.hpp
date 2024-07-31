#ifndef QRK_WINDOW
#define QRK_WINDOW

#include "../dependencies/OpenGL.h"
#include "../include/color.hpp"
#include "../include/qrk_debug.hpp"
#include "../include/vector.hpp"
#include <Windows.h>
#include <string>
#include <windowsx.h>

#define Q_WINDOW_DEFAULT WS_OVERLAPPEDWINDOW
#define Q_WINDOW_NONRESIZABLE WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU

namespace qrk {
struct WindowSettings {
    int windowStyle = Q_WINDOW_DEFAULT;
    int multisamplingLevel = 8;
    qrk::Color clearColor = {255, 255, 255, 255};
    int glMajorVersion = 4;
    int glMinorVersion = 6;
};
class GLTools {
public:
    //context creation tools
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;
};

class glWindow : GLTools {
public:
    glWindow(const std::string &windowName, qrk::vec2u size,
             qrk::WindowSettings settings =
                     {Q_WINDOW_DEFAULT, 8, {255, 255, 255, 255}, 4, 6})
        : Open(true), windowSize(size), mouseMovedCallback(nullptr) {
        Create(windowName, size, settings.windowStyle,
               settings.multisamplingLevel, settings.clearColor,
               settings.glMajorVersion, settings.glMinorVersion);
    }
    ~glWindow() {}
    void Activate() const { SetActiveWindow(window); }
    bool IsOpen() const { return Open; }
    void Close() {
        wglDeleteContext(this->glContext);
        DestroyWindow(this->window);
        this->Open = false;
    }
    bool IsContextCurrent() {
        if (wglGetCurrentContext() == glContext) {
            return true;
        } else {
            return false;
        }
    }
    void MakeContextCurrent() const {
        wglMakeCurrent(deviceContext, glContext);
    }
    void SetClearColor(qrk::Color _clearColor) {
        qrk::ColorF fColor = qrk::ConvertToFloat(_clearColor);
        glClearColor(fColor.r, fColor.g, fColor.b, fColor.a);
    }
    void Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
    void SwapWindowBuffers() const { SwapBuffers(deviceContext); }
    void SetSwapInterval(int interval) { wglSwapIntervalEXT(interval); }

    //get and set window parameters
    MSG GetWindowMessage() const {
        MSG msg = {};
        if (PeekMessageW(&msg, this->window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return msg;
    }
    void q_HideCursor() { ShowCursor(FALSE); }
    void q_ShowCursor() { ShowCursor(TRUE); }
    qrk::vec2u GetSize() const {
        if(!IsOpen()) {
            return qrk::vec2u({0, 0});
        }
        RECT windowRect;
        if (!GetClientRect(window, &windowRect)) {
            std::string error =
                    "Could not retrieve window size information. Error code: " +
                    std::to_string(GetLastError());
            qrk::debug::Error(error, qrk::debug::Q_RUNTIME_ERROR);
        }
        return qrk::vec2u(
                {(unsigned int) (windowRect.right - windowRect.left),
                 (unsigned int) (windowRect.bottom - windowRect.top)});
    }
    void SetSize(qrk::vec2u newSize) const {
        if (!SetWindowPos(window, HWND_TOP, 0, 0, newSize.x(), newSize.y(),
                          SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOACTIVATE)) {
            std::string error = std::to_string(GetLastError());
            qrk::debug::ShowErrorBox(error);
            qrk::debug::LogError(error);
        }
    }

    qrk::vec2u GetPosition() const {
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
    void SetPosition(qrk::vec2u newPos) const {
        if (!SetWindowPos(window, HWND_TOP, 0, 0, newPos.x(), newPos.y(),
                          SWP_NOSIZE | SWP_NOREPOSITION | SWP_NOACTIVATE)) {
            std::string error = std::to_string(GetLastError());
            qrk::debug::ShowErrorBox(error);
            qrk::debug::LogError(error);
        }
    }
    void SetMouseMovedCallback(void (*callback)(qrk::vec2i mousePosition)) {
        this->mouseMovedCallback = callback;
    }
    HWND GetNativeWindowHandle() const { return this->window; }
    HDC GetNativeDeviceContextHandle() const { return this->deviceContext; }
    HGLRC GetNativeContextHandle() const { return this->glContext; }

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

private:
    bool Create(const std::string &windowName, qrk::vec2u size, int windowStyle,
                int multisamplingLevel, qrk::Color _clearColor,
                int glMajorVersion, int glMinorVersion);
    bool CreateContext(int multisamplingLevel, int glMajorVersion,
                       int glMinorVersion);
};
}// namespace qrk

#endif// !QRK_WINDOW