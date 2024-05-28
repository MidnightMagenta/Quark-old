#ifndef QRK_WINDOW
#define QRK_WINDOW

#include <Windows.h>
#include <iostream>
#include <string>
#include "../dependencies/OpenGL.h"
#include "../include/vector.hpp"

namespace qrk
{
	class GLTools
	{
	public:
		//context creation tools 
		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;

		PFNGLGENBUFFERSPROC _glGenBuffers = nullptr;
		PFNGLDELETEBUFFERSPROC _glDeleteBuffers = nullptr;
		PFNGLDELETEVERTEXARRAYSPROC _glDeleteVertexArrays = nullptr;
		PFNGLBINDBUFFERPROC _glBindBuffer = nullptr;
		PFNGLBINDBUFFERRANGEPROC _glBindBufferRange = nullptr;
		PFNGLBUFFERDATAPROC _glBufferData = nullptr;
		PFNGLGENVERTEXARRAYSPROC _glGenVertexArrays = nullptr;
		PFNGLBINDVERTEXARRAYPROC _glBindVertexArray = nullptr;
		PFNGLENABLEVERTEXATTRIBARRAYPROC _glEnableVertexAttribArray = nullptr;
		PFNGLVERTEXATTRIBPOINTERPROC _glVertexAttribPointer = nullptr;
		PFNGLVERTEXATTRIBIPOINTERPROC _glVertexAttribIPointer = nullptr;
		PFNGLVERTEXATTRIB3FPROC _glVertexAttrib3f = nullptr;
		PFNGLVERTEXATTRIB4FPROC _glVertexAttrib4f = nullptr;
		PFNGLDELETEPROGRAMPROC _glDeleteProgram = nullptr;
		PFNGLDELETESHADERPROC _glDeleteShader = nullptr;
		PFNGLCREATESHADERPROC _glCreateShader = nullptr;
		PFNGLSHADERSOURCEPROC _glShaderSource = nullptr;
		PFNGLCOMPILESHADERPROC _glCompileShader = nullptr;
		PFNGLCREATEPROGRAMPROC _glCreateProgram = nullptr;
		PFNGLATTACHSHADERPROC _glAttachShader = nullptr;
		PFNGLDETACHSHADERPROC _glDetachShader = nullptr;
		PFNGLLINKPROGRAMPROC _glLinkProgram = nullptr;
		PFNGLUSEPROGRAMPROC _glUseProgram = nullptr;
		PFNGLBINDATTRIBLOCATIONPROC _glBindAttribLocation = nullptr;
		PFNGLBINDFRAGDATALOCATIONPROC _glBindFragDataLocation = nullptr;
		PFNGLGETFRAGDATALOCATIONPROC _glGetFragDataLocation = nullptr;
		PFNGLGETUNIFORMLOCATIONPROC _glGetUniformLocation = nullptr;
		PFNGLGETSHADERIVPROC _glGetShaderiv = nullptr;
		PFNGLGETSHADERINFOLOGPROC _glGetShaderInfoLog = nullptr;
		PFNGLDRAWBUFFERSPROC _glDrawBuffers = nullptr;
		PFNGLUNIFORM4FPROC _glUniform4f = nullptr;
		PFNGLUNIFORM4FVPROC _glUniform4fv = nullptr;
		PFNGLUNIFORM3FVPROC _glUniform3fv = nullptr;
		PFNGLUNIFORM2FVPROC _glUniform2fv = nullptr;
		PFNGLUNIFORM3FPROC _glUniform3f = nullptr;
		PFNGLUNIFORM2FPROC _glUniform2f = nullptr;
		PFNGLUNIFORM1FPROC _glUniform1f = nullptr;
		PFNGLUNIFORM1IPROC _glUniform1i = nullptr;
		PFNGLUNIFORMMATRIX4FVPROC _glUniformMatrix4fv = nullptr;
		PFNGLUNIFORMMATRIX3FVPROC _glUniformMatrix3fv = nullptr;
		PFNGLGETPROGRAMIVPROC _glGetProgramiv = nullptr;
		PFNGLGETACTIVEUNIFORMNAMEPROC _glGetActiveUniformName = nullptr;
		PFNGLGETACTIVEUNIFORMSIVPROC _glGetActiveUniformsiv = nullptr;
		PFNGLGETACTIVEUNIFORMPROC _glGetActiveUniform = nullptr;
		PFNGLDRAWELEMENTSBASEVERTEXPROC _glDrawElementsBaseVertex = nullptr;
		PFNGLTEXIMAGE2DMULTISAMPLEPROC _glTexImage2DMultisample = nullptr;
		PFNGLACTIVETEXTUREPROC _glActiveTexture = nullptr;
		PFNGLGENERATEMIPMAPPROC _glGenerateMipmap = nullptr;
		PFNGLMAPBUFFERPROC _glMapBuffer = nullptr;
		PFNGLMAPBUFFERRANGEPROC _glMapBufferRange = nullptr;
		PFNGLUNMAPBUFFERPROC _glUnmapBuffer = nullptr;
		PFNGLGENRENDERBUFFERSPROC _glGenRenderbuffers = nullptr;
		PFNGLDELETERENDERBUFFERSPROC _glDeleteRenderbuffers = nullptr;
		PFNGLBINDRENDERBUFFERPROC _glBindRenderbuffer = nullptr;
		PFNGLRENDERBUFFERSTORAGEPROC _glRenderbufferStorage = nullptr;
		PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC _glRenderbufferStorageMultisample = nullptr;
		PFNGLCOPYBUFFERSUBDATAPROC _glCopyBufferSubData = nullptr;
		PFNGLBUFFERSUBDATAPROC _glBufferSubData = nullptr;
		PFNGLGENFRAMEBUFFERSPROC _glGenFramebuffers = nullptr;
		PFNGLDELETEFRAMEBUFFERSPROC _glDeleteFramebuffers = nullptr;
		PFNGLBINDFRAMEBUFFERPROC _glBindFramebuffer = nullptr;
		PFNGLFRAMEBUFFERTEXTURE2DPROC _glFramebufferTexture2D = nullptr;
		PFNGLFRAMEBUFFERRENDERBUFFERPROC _glFramebufferRenderbuffer = nullptr;
		PFNGLCHECKFRAMEBUFFERSTATUSPROC _glCheckFramebufferStatus = nullptr;
		PFNGLBLITFRAMEBUFFERPROC _glBlitFramebuffer = nullptr;
		PFNGLBLENDEQUATIONPROC _glBlendEquation = nullptr;
	};

	class Window : GLTools
	{
	public:
		Window(std::string windowName, qrk::vec2u size, int windowStyle) { Create(windowName, size, windowStyle); }
		bool Create(std::string windowName, qrk::vec2u size, int windowStyle);
		bool CreateContext();

		//callbacks
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
		void LoadAllExtensions();
	};
}

#endif // !QRK_WINDOW