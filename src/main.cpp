#include <../include/draw.hpp>
#include <../include/object.hpp>
#include <../include/units.hpp>
#include <../include/window.hpp>
#include <iostream>

//define entry point of the application
int run() {
    qrk::glWindow wnd("Planet-Demo",
                      qrk::vec2u({(unsigned int) 800, (unsigned int) 800}),
                      Q_WINDOW_DEFAULT, 8, qrk::Color{10, 10, 10, 255});
    wnd.MakeContextCurrent();

    qrk::Object obj("objects/uv_sphere.obj");
    wnd.ChangeClearColor({255, 0, 0, 255});
    while (obj.WaitForLoad()) {
        wnd.GetWindowMessage();
        wnd.Clear();
        wnd.SwapWindowBuffers();
    }
    wnd.ChangeClearColor({10, 10, 10, 255});

    qrk::Texture2D texture("textures/earth.jpg");
    qrk::Texture2D texture2("textures/mercury.jpg", qrk::Texture2DSettings{});
    qrk::settings stng;
    stng.fov = 90 * qrk::units::deg;
    qrk::Renderer render(wnd, &stng);
    qrk::GLObject sun(obj);
    qrk::GLObject planet(obj);
    obj.DeleteData();
    sun.SetScale(0.8f, 0.8f, 0.8f);
    sun.SetPosition(0, 0, -5.f);
    sun.SetTexture(texture);

    planet.SetScale(0.3f, 0.3f, 0.3f);
    planet.SetPosition(3.f, 0.f, -5.f);
    planet.SetTexture(texture2);

    qrk::LightSource ls;
    ls.lightSourceColor = qrk::vec4f({1, 1, 1, 1});
    ls.lightSourcePosition = qrk::vec3f({6, 6, 0});
    render.AddPointLightSource(ls);
    ls.lightSourceColor = qrk::vec4f({0.2, 0.2, 0.2, 1});
    ls.lightSourcePosition = qrk::vec3f({-6, -6, 0});
    render.AddPointLightSource(ls);

    int fc = 0;
    while (wnd.IsOpen()) {
        wnd.GetWindowMessage();
        wnd.Clear();

        sun.SetRotation(180 * qrk::units::deg, -fc * qrk::units::deg * 0.4f, 0);
        planet.SetRotation(0 * qrk::units::deg, fc * qrk::units::deg * 0.7f, 0 * qrk::units::deg);
        planet.SetPosition(std::sin((float)fc / 100) * 3.f, std::cos((float)fc / 100) * -0.5f, std::cos((float)fc / 100) * 3.f - 5.f);
        render.QueueDraw(sun.GetDrawData(), qrk::DrawTarget::Q_3D_DRAW);
        render.QueueDraw(planet.GetDrawData(), qrk::DrawTarget::Q_3D_DRAW);
        render.Draw();

        wnd.SwapWindowBuffers();
        fc++;
    }

    return 0;
}

int main() {
    try {
        return run();
    } catch (...) { return -1; }
}