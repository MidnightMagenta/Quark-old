#ifndef QRK_WINDOW
#define QRK_WINDOW

#include <Windows.h>
#include <iostream>
#include <string>
#include <gl/GL.h>
#include "../dependencies/wglext.h"
#include "../include/vector.hpp"

namespace qrk
{
	class GlContextTools
	{
	public:
		//context creation tools 
		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
	};

	class Window : GlContextTools
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