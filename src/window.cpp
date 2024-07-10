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
            if (GetClientRect(this->window, &rect)) {
                qrk::vec2u size(
                        {(unsigned int) rect.right - (unsigned int) rect.left,
                         (unsigned int) rect.bottom - (unsigned int) rect.top});
                glViewport(0, 0, size.x(), size.y());
                this->windowSize = size;
            }
            break;
        case WM_CLOSE:
            DestroyWindow(this->window);
            this->Open = false;
            return NULL;
        case WM_ACTIVATE:
            if (wParam == WA_CLICKACTIVE) { MakeContextCurrent(); }
            break;
        case WM_MOUSEMOVE:
            if (mouseMovedCallback != nullptr) {
                mouseMovedCallback(qrk::vec2i(
                        {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)}));
            }
            break;
        default:
            break;
    }
    return DefWindowProc(this->window, message, wParam, lParam);
}

bool qrk::glWindow::Create(const std::string &_windowName, qrk::vec2u _size,
                           int windowStyle, int multisamplingLevel,
                           qrk::Color _clearColor, int glMajorVersion,
                           int glMinorVersion) {
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = Process;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = GetModuleHandleA(NULL);
    windowClass.hIcon = NULL;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground =
            CreateSolidBrush(RGB(_clearColor.r, _clearColor.g, _clearColor.b));
    windowClass.lpszMenuName = NULL;
    std::string className = _windowName + "-CLASS";
    windowClass.lpszClassName = className.c_str();
    windowClass.hIconSm = NULL;
    RegisterClassEx(&windowClass);

    window = CreateWindowExA(0, className.c_str(), _windowName.c_str(),
                             windowStyle, CW_USEDEFAULT, CW_USEDEFAULT,
                             _size.x(), _size.y(), NULL, NULL,
                             GetModuleHandleA(0), this);
    if (window == NULL) {
        qrk::debug::Error("Failed to create a window",
                          qrk::debug::Q_FAILED_TO_CREATE_WINDOW);
    }
    if (!CreateContext(multisamplingLevel, glMajorVersion, glMinorVersion)) {
        qrk::debug::Error("Failed to create context",
                          qrk::debug::Q_FAILED_TO_CREATE_CONTEXT);
    }
    ShowWindow(window, SW_SHOW);
    qrk::ColorF fColor = qrk::ConvertToFloat(_clearColor);
    glClearColor(fColor.r, fColor.g, fColor.b, fColor.a);
    return true;
}

bool qrk::glWindow::CreateContext(int multisamplingLevel, int glMajorVersion,
                                  int glMinorVersion) {
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
        qrk::debug::ShowErrorBox("Failed to create dummy window");
        qrk::debug::LogError("Failed to create dummy window");
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
        qrk::debug::ShowErrorBox("Failed to create dummy context");
        qrk::debug::LogError("Failed to create dummy context");
        return false;
    }

    if (!wglMakeCurrent(dummyDeviceHandle, dummyContext)) {
        qrk::debug::ShowErrorBox("Failed to make dummy context current");
        qrk::debug::LogError("Failed to make dummy context current");
        return false;
    }

    //create real context
    LoadContextCreationTools();

    const int contextAttribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,
                                  glMajorVersion,
                                  WGL_CONTEXT_MINOR_VERSION_ARB,
                                  glMinorVersion,
                                  WGL_CONTEXT_PROFILE_MASK_ARB,
                                  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                                  0};

    const int pixelFormatAttributes[] = {WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                                         //////////////////////
                                         WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                                         //////////////////////
                                         WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                                         //////////////////////
                                         WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                                         //////////////////////
                                         WGL_COLOR_BITS_ARB, 32,
                                         //////////////////////
                                         WGL_DEPTH_BITS_ARB, 24,
                                         //////////////////////
                                         WGL_STENCIL_BITS_ARB, 8,
                                         //////////////////////
                                         WGL_SAMPLE_BUFFERS_ARB, 1,
                                         //////////////////////
                                         WGL_SAMPLES_ARB, multisamplingLevel,
                                         //////////////////////
                                         0};
    int msPixelFormat;
    UINT numFormats;
    deviceContext = GetDC(window);

    if (!wglChoosePixelFormatARB(deviceContext, pixelFormatAttributes, NULL, 1,
                                 &msPixelFormat, &numFormats)) {
        qrk::debug::ShowErrorBox("Failed to create modernGl context (PF)");
        qrk::debug::LogError("Failed to create modernGl context (PF)");
        return false;
    }
    SetPixelFormat(deviceContext, msPixelFormat, &pfd);
    glContext = wglCreateContextAttribsARB(deviceContext, 0, contextAttribs);
    if (glContext == 0) {
        qrk::debug::ShowErrorBox("Failed to create modernGl context");
        qrk::debug::LogError("Failed to create modernGl context");
        return false;
    }
    //clean up
    wglMakeCurrent(dummyDeviceHandle, nullptr);
    wglDeleteContext(dummyContext);
    ReleaseDC(dummyWindow, dummyDeviceHandle);
    DestroyWindow(dummyWindow);

    if (!wglMakeCurrent(deviceContext, glContext)) {
        qrk::debug::ShowErrorBox("Failed to make modernGl context current");
        qrk::debug::LogError("Failed to make modernGl context current");
        return false;
    }

    if (!gladLoadGL()) {
        qrk::debug::ShowErrorBox("Could not initialize GLAD");
        qrk::debug::LogError("Could not initialize GLAD");
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
        wglChoosePixelFormatARB == nullptr || wglSwapIntervalEXT == nullptr) {
        qrk::debug::Error("Failed to load context creation tools",
                          qrk::debug::Q_FAILED_TO_CREATE_CONTEXT);
    }
}