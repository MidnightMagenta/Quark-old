#include <../include/draw.hpp>
#include <../include/object.hpp>
#include <../include/units.hpp>
#include <../include/window.hpp>
#include <../include/event.hpp>
#include <iostream>

//define entry point of the application
int run() {
    qrk::glWindow wnd("Planet-Demo",
                      qrk::vec2u({(unsigned int) 800, (unsigned int) 800}),
                      Q_WINDOW_DEFAULT, 8, qrk::Color({10, 10, 10, 255}));
    wnd.MakeContextCurrent();
    wnd.SetSwapInterval(1);
    qrk::Object obj("objects/smooth_uv_sphere.obj");
    wnd.ChangeClearColor(qrk::Q_COLOR::RED);
    while (obj.WaitForLoad()) {
        wnd.GetWindowMessage();
        wnd.Clear();
        wnd.SwapWindowBuffers();
    }
    wnd.ChangeClearColor({10, 10, 10, 255});
    qrk::Texture2D texture("textures/testTexture.png");
    qrk::Texture2D texture2("textures/mercury.jpg", qrk::Texture2DSettings{});
    qrk::settings stng;
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

    qrk::Debug::FrameCounter fc_d;
    int fc = 0;
    int fc2 = 0;
    int fb = 10;
    qrk::Event e(wnd);
    while (wnd.IsOpen()) {
        e.UpdateWindow();
        wnd.Clear();

        sun.SetRotation(180 * qrk::units::deg, -fc * qrk::units::deg * 0.4f, 0);
        planet.SetRotation(0 * qrk::units::deg, fc * qrk::units::deg * -0.4f,
                           0 * qrk::units::deg);
        planet.SetPosition(std::sin((float) fc / 100) * 3.f,
                           std::cos((float) fc / 100) * -0.5f,
                           std::cos((float) fc / 100) * 3.f - 5.f);
        render.Queue3dDraw(sun.GetDrawData());
        render.Queue3dDraw(planet.GetDrawData());

        render.Draw();

        wnd.SwapWindowBuffers();
        fc++;
        fc2++;
        if (fc2 >= fb) {
            std::cout << fc_d.GetFrameRate() * fb << " FPS     " << std::endl;
            qrk::Debug::set_cursor(0, 0);
            fc2 = 0;
        }
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