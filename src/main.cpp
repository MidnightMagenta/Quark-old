#include <../include/draw.hpp>
#include <../include/event.hpp>
#include <../include/object.hpp>
#include <../include/rect.hpp>
#include <../include/units.hpp>
#include <../include/window.hpp>
#include <iostream>

//define entry point of the application
int run() {
    qrk::WindowSettings stngs;
    stngs.clearColor = {10, 10, 10, 255};
    qrk::glWindow wnd("Planet-Demo",
                      qrk::vec2u({(unsigned int) 800, (unsigned int) 800}), stngs);
    wnd.MakeContextCurrent();
    wnd.SetSwapInterval(1);
    qrk::Object obj("resources/objects/cube.obj");
    wnd.SetClearColor(qrk::Q_COLOR::RED);
    while (obj.WaitForLoad(wnd)) {
        wnd.Clear();
        wnd.SwapWindowBuffers();
    }
    wnd.SetClearColor({10, 10, 10, 255});
    qrk::Texture2D texture("resources/textures/testTexture.png");
    qrk::RendererSettings stng;
    stng.fov = 90 * qrk::units::deg;
    qrk::qb_GL_Renderer render(wnd, &stng);
    qrk::GLObject sun(obj);
    qrk::GLObject planet(obj);
    obj.DeleteData();
    sun.SetScale(0.8f, 0.8f, 0.8f);
    sun.SetPosition(0, 0, -5.f);
    sun.SetTexture(texture);

    planet.SetScale(0.3f, 0.3f, 0.3f);
    planet.SetPosition(3.f, 0.f, -5.f);
    planet.SetTexture(texture);

    qrk::LightSource ls;
    ls.position = qrk::vec3f({3.f, 5.f, 0.f});
    render.AddPointLightSource(ls);

    qrk::debug::FrameCounter fc_d;
    int fc = 0;
    qrk::Event e(wnd);

    qrk::Rect rect;
    rect.SetSize(400, 400);
    rect.SetPosition(200, 200);
    rect.SetOffset(-rect.GetSize().x() / 2, -rect.GetSize().y() / 2);
    rect.SetTexture(texture);

    while (wnd.IsOpen()) {
        e.UpdateWindow();
        if (e.KeyDown(qrk::ESCAPE)) { wnd.Close(); }
        wnd.Clear();

        //sun.SetRotation(180 * qrk::units::deg, -fc * qrk::units::deg * 0.4f, 0);
        //planet.SetRotation(0 * qrk::units::deg, fc * qrk::units::deg * -0.4f,
        //                   0 * qrk::units::deg);
        //planet.SetPosition(std::sin((float) fc / 100) * 3.f,
        //                   std::cos((float) fc / 100) * -0.5f,
        //                   std::cos((float) fc / 100) * 3.f - 5.f);
        //render.Queue3dDraw(sun.GetDrawData());
        //render.Queue3dDraw(planet.GetDrawData());
        render.QueueDraw(rect.GetDrawData());

        render.Draw();
        wnd.SwapWindowBuffers();
        fc++;
    }
    return 0;
}

int main() {
    try {
        return run();
    } catch (std::exception &e) { return std::stoi(e.what()); } catch (...) {
        return -1;
    }
}