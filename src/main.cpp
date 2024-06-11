#include <../include/window.hpp>
#include <../include/object.hpp>
#include <iostream>

int main()
{
	qrk::glWindow wnd("Test", qrk::vec2u({ (unsigned int)800, (unsigned int)800 }), WS_OVERLAPPEDWINDOW, { 0, 100, 255, 255 });
	wnd.MakeContextCurrent();

	qrk::Object obj("objects/cube.obj");

	while (obj.WaitForLoad())
	{
		wnd.GetWindowMessage();
		wnd.Clear({ 255, 0, 0, 255 });
		wnd.SwapWindowBuffers();
	}

	qrk::assets::Program testProgram("shaders/3d_vertex_shader.vert", "shaders/3d_fragment_shader.frag");

	while (wnd.IsOpen()) {
		wnd.GetWindowMessage();
		wnd.Clear({ 0, 100, 255, 255 });
		wnd.SwapWindowBuffers();
	}

	return 0;
}