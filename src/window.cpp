#include "../include/window.hpp"

LRESULT CALLBACK qrk::glWindow::Process(HWND hWnd, UINT message, WPARAM wParam,
                                        LPARAM lParam) {
    qrk::glWindow *pThis;
    if (message == WM_CREATE) {
        CREATESTRUCT *pCreate = (CREATESTRUCT *) lParam;
        pThis = (qrk::glWindow *) pCreate->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) pThis);
        pThis->window = hWnd;
    } else {
        pThis = (qrk::glWindow *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    if (pThis) {
        return pThis->WndProcess(message, wParam, lParam);
    } else
        return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT qrk::glWindow::WndProcess(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_SIZE:
            RECT rect;
            if (GetWindowRect(this->window, &rect)) {
                qrk::vec2u size(
                        {(unsigned int) rect.right - (unsigned int) rect.left,
                         (unsigned int) rect.bottom - (unsigned int) rect.top});
                glViewport(0, 0, size.x(), size.y());
                this->windowSize = size;
            }
            return DefWindowProc(this->window, message, wParam, lParam);
        case WM_CLOSE:
            DestroyWindow(this->window);
            this->Open = false;
            return NULL;
        case WM_ACTIVATE:
            if (wParam == WA_CLICKACTIVE) { MakeContextCurrent(); }
            return DefWindowProc(this->window, message, wParam, lParam);
        default:
            return DefWindowProc(this->window, message, wParam, lParam);
    }
}

bool qrk::glWindow::Create(const std::string &windowName, qrk::vec2u size,
                           int windowStyle, int multisamplingLevel,
                           qrk::Color resizeColor) {
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = Process;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = GetModuleHandleA(NULL);
    windowClass.hIcon = NULL;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground =
            CreateSolidBrush(RGB(resizeColor.r, resizeColor.g, resizeColor.b));
    windowClass.lpszMenuName = NULL;
    std::string className = windowName + "-CLASS";
    windowClass.lpszClassName = className.c_str();
    windowClass.hIconSm = NULL;
    RegisterClassEx(&windowClass);

    window =
            CreateWindowExA(0, className.c_str(), windowName.c_str(),
                            windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, size.x(),
                            size.y(), NULL, NULL, GetModuleHandleA(0), this);
    if (window == NULL) {
        qrk::Debug::ShowErrorBox("Failed to create a window");
        qrk::Debug::LogError("Failed to create a window");
        throw std::exception();
    }
    if (!CreateContext(multisamplingLevel)) throw std::exception();
    ShowWindow(window, SW_SHOW);
    return true;
}

bool qrk::glWindow::CreateContext(int multisamplingLevel) {
    //courtesy of AngeTheGreat
    //create dummy window
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = DummyProcess;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandleA(NULL);
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "DUMMY_WINDOW";
    wc.hIconSm = NULL;
    RegisterClassEx(&wc);

    HWND dummyWindow =
            CreateWindowExW(0, L"DUMMY_WINDOW", L"DUMMY_WINDOW", 0, 0, 0, 0, 0,
                            0, 0, GetModuleHandleA(0), 0);
    if (dummyWindow == NULL) {
        qrk::Debug::ShowErrorBox("Failed to create dummy window");
        qrk::Debug::LogError("Failed to create dummy window");
        return false;
    }
    HDC dummyDeviceHandle = GetDC(dummyWindow);

    PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR),
                                 1,
                                 PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
                                         PFD_DOUBLEBUFFER,
                                 PFD_TYPE_RGBA,
                                 32,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 24,
                                 8,
                                 0,
                                 PFD_MAIN_PLANE,
                                 0,
                                 0,
                                 0,
                                 0};

    int pixelFormat = ChoosePixelFormat(dummyDeviceHandle, &pfd);
    SetPixelFormat(dummyDeviceHandle, pixelFormat, &pfd);
    HGLRC dummyContext = wglCreateContext(dummyDeviceHandle);
    if (dummyContext == nullptr) {
        qrk::Debug::ShowErrorBox("Failed to create dummy context");
        qrk::Debug::LogError("Failed to create dummy context");
        return false;
    }

    if (!wglMakeCurrent(dummyDeviceHandle, dummyContext)) {
        qrk::Debug::ShowErrorBox("Failed to make dummy context current");
        qrk::Debug::LogError("Failed to make dummy context current");
        return false;
    }

    //create real context
    LoadContextCreationTools();

    const int contextAttribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,
                                  4,
                                  WGL_CONTEXT_MINOR_VERSION_ARB,
                                  6,
                                  WGL_CONTEXT_PROFILE_MASK_ARB,
                                  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                                  0};

    const int pixelFormatAttributes[] = {WGL_DRAW_TO_WINDOW_ARB,
                                         GL_TRUE,
                                         //////////////////////
                                         WGL_SUPPORT_OPENGL_ARB,
                                         GL_TRUE,
                                         //////////////////////
                                         WGL_DOUBLE_BUFFER_ARB,
                                         GL_TRUE,
                                         //////////////////////
                                         WGL_PIXEL_TYPE_ARB,
                                         WGL_TYPE_RGBA_ARB,
                                         //////////////////////
                                         WGL_COLOR_BITS_ARB,
                                         32,
                                         //////////////////////
                                         WGL_DEPTH_BITS_ARB,
                                         24,
                                         //////////////////////
                                         WGL_STENCIL_BITS_ARB,
                                         8,
                                         //////////////////////
                                         WGL_SAMPLE_BUFFERS_ARB,
                                         1,
                                         //////////////////////
                                         WGL_SAMPLES_ARB,
                                         multisamplingLevel,
                                         //////////////////////
                                         0};
    int msPixelFormat;
    UINT numFormats;
    deviceContext = GetDC(window);

    if (!wglChoosePixelFormatARB(deviceContext, pixelFormatAttributes, NULL, 1,
                                 &msPixelFormat, &numFormats)) {
        qrk::Debug::ShowErrorBox("Failed to create modernGl context (PF)");
        qrk::Debug::LogError("Failed to create modernGl context (PF)");
        return false;
    }
    SetPixelFormat(deviceContext, msPixelFormat, &pfd);
    glContext = wglCreateContextAttribsARB(deviceContext, 0, contextAttribs);
    if (glContext == 0) {
        qrk::Debug::ShowErrorBox("Failed to create modernGl context");
        qrk::Debug::LogError("Failed to create modernGl context");
        return false;
    }
    //clean up
    wglMakeCurrent(dummyDeviceHandle, nullptr);
    wglDeleteContext(dummyContext);
    ReleaseDC(dummyWindow, dummyDeviceHandle);
    DestroyWindow(dummyWindow);

    if (!wglMakeCurrent(deviceContext, glContext)) {
        qrk::Debug::ShowErrorBox("Failed to make modernGl context current");
        qrk::Debug::LogError("Failed to make modernGl context current");
        return false;
    }

    if (!gladLoadGL()) {
        qrk::Debug::ShowErrorBox("Could not initialize GLAD");
        qrk::Debug::LogError("Could not initialize GLAD");
        return false;
    }
    return true;
}

void qrk::glWindow::LoadContextCreationTools() {
    wglCreateContextAttribsARB =
            (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress(
                    "wglCreateContextAttribsARB");
    wglChoosePixelFormatARB =
            (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress(
                    "wglChoosePixelFormatARB");
    wglSwapIntervalEXT =
            (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
    if (wglCreateContextAttribsARB == nullptr ||
        wglChoosePixelFormatARB == nullptr) {
        MessageBox(0, "Failed to load context creation tools", "Error", 0);
        qrk::Debug::ShowErrorBox("Failed to load context creation tools");
        qrk::Debug::LogError("Failed to load context creation tools");
        throw;
    }
}