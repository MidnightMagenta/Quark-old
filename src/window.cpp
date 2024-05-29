#include "../include/window.hpp"

LRESULT CALLBACK qrk::Window::Process(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	qrk::Window* pThis;
	if (message == WM_CREATE)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		pThis = (qrk::Window*)pCreate->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
		pThis->window = hWnd;
	}
	else {
		pThis = (qrk::Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	if (pThis)
	{
		return pThis->WndProcess(message, wParam, lParam);
	}
	else return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT qrk::Window::WndProcess(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		RECT rect;
		if (GetWindowRect(this->window, &rect))
		{
			qrk::vec2i size({ rect.right - rect.left, rect.bottom - rect.top });
		}
		return DefWindowProc(this->window, message, wParam, lParam);
	default:
		return DefWindowProc(this->window, message, wParam, lParam);
	}
}

bool qrk::Window::Create(std::string windowName, qrk::vec2u size, int windowStyle)
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = Process;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	windowClass.lpszMenuName = NULL;
	std::string className = windowName + "-CLASS";
	windowClass.lpszClassName = className.c_str();
	windowClass.hIconSm = NULL;
	RegisterClassEx(&windowClass);

	window = CreateWindowExA(
		0, className.c_str(), windowName.c_str(),
		windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, size.x(), size.y(),
		NULL,
		NULL,
		GetModuleHandleA(0),
		this
	);
	if (window == NULL)
	{
		MessageBox(0, "Failed to create window", "Error", MB_OK);
		return false;
	}

	deviceContext = GetDC(window);
	if (!CreateContext()) return false;
	ShowWindow(window, SW_SHOW);
	return true;
}

bool qrk::Window::CreateContext()
{
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
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "DUMMY_WINDOW";
	wc.hIconSm = NULL;
	RegisterClassEx(&wc);

	HWND dummyWindow = CreateWindowExW(0, L"DUMMY_WINDOW", L"DUMMY_WINDOW", 0, 0, 0, 0, 0, 0, 0, GetModuleHandleA(0), 0);
	if (dummyWindow == NULL)
	{
		MessageBox(0, "Failed to create dummy window", "Error", MB_OK);
		return false;
	}
	HDC dummyDeviceHandle = GetDC(dummyWindow);

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,
		8,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int pixelFormat = ChoosePixelFormat(dummyDeviceHandle, &pfd);
	SetPixelFormat(dummyDeviceHandle, pixelFormat, &pfd);
	HGLRC dummyContext = wglCreateContext(dummyDeviceHandle);
	if (dummyContext == nullptr)
	{
		MessageBox(0, "Failed to create dummy context", "Error", MB_OK);
		return false;
	}

	if (!wglMakeCurrent(dummyDeviceHandle, dummyContext))
	{
		MessageBox(0, "Failed to make dummy context current", "Error", MB_OK);
		return false;
	}
	
	//create real context
	LoadContextCreationTools();

	const int contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	const int pixelFormatAttributes[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_SAMPLE_BUFFERS_ARB, 1,
		WGL_SAMPLES_ARB, 8,
		0
	};
	int msPixelFormat;
	UINT numFormats;
	wglChoosePixelFormatARB(deviceContext, pixelFormatAttributes, NULL, 1, &msPixelFormat, &numFormats);
	SetPixelFormat(deviceContext, msPixelFormat, &pfd);
	glContext = wglCreateContextAttribsARB(deviceContext, 0, contextAttribs);
	if (glContext == 0)
	{
		MessageBox(0, "Failed to create modernGl context", "Error", MB_OK);
		return false;
	}
	if (!wglMakeCurrent(deviceContext, glContext))
	{
		MessageBox(0, "Failed to make modernGl context current", "Error", MB_OK);
		return false;
	}

	//clean up
	wglDeleteContext(dummyContext);
	DestroyWindow(dummyWindow);
	if (!gladLoadGL())
	{
		MessageBox(0, "Could not initialize GLAD", "Error", MB_OK);
		return false;
	}
	//LoadAllExtensions();
	return true;
}

void qrk::Window::LoadContextCreationTools()
{
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if (wglCreateContextAttribsARB == nullptr || wglChoosePixelFormatARB == nullptr)
	{
		MessageBox(0, "Failed to load context creation tools", "Error", 0);
		throw;
	}
}

//void qrk::Window::LoadAllExtensions()
//{
//	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
//	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
//	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
//	glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)wglGetProcAddress("glBindBufferRange");
//	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
//	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
//	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
//	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
//	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
//	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
//	glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)wglGetProcAddress("glVertexAttribIPointer");
//	glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC)wglGetProcAddress("glVertexAttrib3f");
//	glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC)wglGetProcAddress("glVertexAttrib4f");
//
//
//	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
//	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
//
//	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
//	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
//	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
//	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
//	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
//	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
//	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
//	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
//	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
//	glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)wglGetProcAddress("glBindFragDataLocation");
//	glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC)wglGetProcAddress("glGetFragDataLocation");
//	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
//	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
//	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
//	glDrawBuffers = (PFNGLDRAWBUFFERSPROC)wglGetProcAddress("glDrawBuffers");
//
//	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
//	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
//	glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
//	glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
//	glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
//	glUniform2f = (PFNGLUNIFORM2FPROC)wglGetProcAddress("glUniform2f");
//	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
//	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
//
//	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
//	glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress("glUniformMatrix3fv");
//
//	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
//
//	glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC)wglGetProcAddress("glGetActiveUniformName");
//	glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)wglGetProcAddress("glGetActiveUniformsiv");
//	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");
//
//	glMapBuffer = (PFNGLMAPBUFFERPROC)wglGetProcAddress("glMapBuffer");
//	glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)wglGetProcAddress("glMapBufferRange");
//	glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");
//
//	glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC)wglGetProcAddress("glDrawElementsBaseVertex");
//
//
//	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
//	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
//}