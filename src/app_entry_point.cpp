#include <../include/app_entry_point.hpp>

#include <../include/draw.hpp>
#include <../include/object.hpp>
#include <../include/units.hpp>
#include <../include/window.hpp>
#include <iostream>

//define entry point of the application
int run() {
    qrk::glWindow wnd("Test",
                      qrk::vec2u({(unsigned int) 800, (unsigned int) 800}),
                      Q_WINDOW_DEFAULT, 8, {10, 10, 10, 255});
    wnd.MakeContextCurrent();

    qrk::Object obj("objects/2_ico_sphere.obj");

    while (obj.WaitForLoad()) {
        wnd.GetWindowMessage();
        wnd.Clear({255, 0, 0, 255});
        wnd.SwapWindowBuffers();
    }
    qrk::settings stng;
    stng.fov = 70 * qrk::units::deg;
    qrk::Renderer render(wnd, &stng);

    qrk::GLObject drawObj(obj);
    drawObj.SetScale(0.5, 0.5, 0.5);
    drawObj.SetPosition(0, 0, -5);

    int fc = 0;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (wnd.IsOpen()) {
        wnd.GetWindowMessage();
        wnd.Clear({10, 10, 10, 255});

        drawObj.SetRotation(0, fc * qrk::units::deg, 0);

        render.QueueDraw(drawObj.GetDrawData(), qrk::DrawTarget::Q_3D_DRAW);
        render.Draw();

        wnd.SwapWindowBuffers();
        fc++;
    }

    return 0;
}
