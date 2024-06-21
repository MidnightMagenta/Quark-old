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

    qrk::Texture2D texture("textures/testTexture.png");

    qrk::settings stng;
    stng.fov = 70 * qrk::units::deg;
    qrk::Renderer render(wnd, &stng);
    qrk::GLObject drawObj(obj);
    obj.DeleteData();
    drawObj.SetScale(0.6f, 0.6f, 0.6f);
    drawObj.SetPosition(0, 0, -10);
    drawObj.SetTexture(texture);

    int fc = 0;

    while (wnd.IsOpen()) {
        wnd.GetWindowMessage();
        wnd.Clear({10, 10, 10, 255});

        drawObj.SetRotation(-fc * qrk::units::deg * 0.4f, fc * qrk::units::deg,
                            fc * 0.5f * qrk::units::deg);

        render.QueueDraw(drawObj.GetDrawData(), qrk::DrawTarget::Q_3D_DRAW);
        render.Draw();

        wnd.SwapWindowBuffers();
        fc++;
    }

    return 0;
}