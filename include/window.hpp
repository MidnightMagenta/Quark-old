#ifndef QRK_WINDOW
#define QRK_WINDOW

#include <Windows.h>
#include <iostream>
#include <string>
#include "../dependencies/OpenGL.h"
#include "../include/vector.hpp"
#include "../include/color.hpp"

namespace qrk
{
	class GLTools
	{
	public:
		//context creation tools 
		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;

		//PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
		//PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
		//PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
		//PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
		//PFNGLBINDBUFFERRANGEPROC glBindBufferRange = nullptr;
		//PFNGLBUFFERDATAPROC glBufferData = nullptr;
		//PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
		//PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
		//PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
		//PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
		//PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer = nullptr;
		//PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f = nullptr;
		//PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f = nullptr;
		//PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
		//PFNGLDELETESHADERPROC glDeleteShader = nullptr;
		//PFNGLCREATESHADERPROC glCreateShader = nullptr;
		//PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
		//PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
		//PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
		//PFNGLATTACHSHADERPROC glAttachShader = nullptr;
		//PFNGLDETACHSHADERPROC glDetachShader = nullptr;
		//PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
		//PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
		//PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = nullptr;
		//PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation = nullptr;
		//PFNGLGETFRAGDATALOCATIONPROC glGetFragDataLocation = nullptr;
		//PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
		//PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
		//PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
		//PFNGLDRAWBUFFERSPROC glDrawBuffers = nullptr;
		//PFNGLUNIFORM4FPROC glUniform4f = nullptr;
		//PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;
		//PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;
		//PFNGLUNIFORM2FVPROC glUniform2fv = nullptr;
		//PFNGLUNIFORM3FPROC glUniform3f = nullptr;
		//PFNGLUNIFORM2FPROC glUniform2f = nullptr;
		//PFNGLUNIFORM1FPROC glUniform1f = nullptr;
		//PFNGLUNIFORM1IPROC glUniform1i = nullptr;
		//PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
		//PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = nullptr;
		//PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
		//PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName = nullptr;
		//PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv = nullptr;
		//PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = nullptr;
		//PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex = nullptr;
		//PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample = nullptr;
		//PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
		//PFNGLGENERATEMIPMAPPROC glGenerateMipmap = nullptr;
		//PFNGLMAPBUFFERPROC glMapBuffer = nullptr;
		//PFNGLMAPBUFFERRANGEPROC glMapBufferRange = nullptr;
		//PFNGLUNMAPBUFFERPROC glUnmapBuffer = nullptr;
		//PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = nullptr;
		//PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = nullptr;
		//PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = nullptr;
		//PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = nullptr;
		//PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample = nullptr;
		//PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData = nullptr;
		//PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;
		//PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = nullptr;
		//PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = nullptr;
		//PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = nullptr;
		//PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = nullptr;
		//PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = nullptr;
		//PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = nullptr;
		//PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer = nullptr;
		//PFNGLBLENDEQUATIONPROC glBlendEquation = nullptr;
	};

	class Window : GLTools
	{
	public:
		Window(std::string windowName, qrk::vec2u size, int windowStyle) { Create(windowName, size, windowStyle); }
		bool Create(std::string windowName, qrk::vec2u size, int windowStyle);
		bool CreateContext();

		void SwapWindowBuffers() { SwapBuffers(deviceContext); }
		void MakeContextCurrent() { wglMakeCurrent(deviceContext, glContext); }
		void Clear(qrk::Color color)
		{
			qrk::ColorF fColor = qrk::ConvertToFloat(color);
			glClearColor(fColor.r, fColor.g, fColor.b, fColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

	private:
		WNDCLASSEX windowClass;
		HWND window;
		HDC deviceContext;
		HGLRC glContext;

		static LRESULT CALLBACK Process(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK DummyProcess(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		LRESULT WndProcess(UINT message, WPARAM wParam, LPARAM lParam);

		void LoadContextCreationTools();
		//void LoadAllExtensions();
	};
}

#endif // !QRK_WINDOW