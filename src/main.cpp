#include <../include/window.hpp>
#include <iostream>

int main()
{
	qrk::Window wnd("Test", qrk::vec2u({(unsigned int)400, (unsigned int)400 }), WS_OVERLAPPEDWINDOW);
    std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;
    wnd.MakeContextCurrent();
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        wnd.Clear({ 255, 100, 60, 255 });
        wnd.SwapWindowBuffers();
    }
    
    return 0;
}