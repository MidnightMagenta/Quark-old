#include <../include/event.hpp>
#include <../include/object.hpp>
#include <../include/rect.hpp>
#include <../include/render_window.hpp>
#include <../include/units.hpp>
#include <iostream>

//define entry point of the application
int run() {
    qrk::RenderWindowSettings rws;
    rws.windowSettings.clearColor = {10, 10, 10, 255};
    qrk::RenderWindow window(qrk::vec2u({800, 800}), "TestWindow", rws);
    qrk::Texture2D texture("resources/textures/testTexture.png");
    qrk::Event e(window.GetWindow());

    qrk::Rect rect;
    rect.SetSize(400, 400);
    rect.SetPosition(200, 200);
    rect.SetOffset(-rect.GetSize().x() / 2, -rect.GetSize().y() / 2);
    rect.SetTexture(texture);

    while (window.IsOpen()) {
        e.UpdateWindow();
        if (e.KeyDown(qrk::ESCAPE)) { window.Close(); }
        window.ClearWindow();
        window.QueueDraw(rect.GetDrawData());
        window.Draw();
    }
    return 1;
}

int main() {
    try {
        return run();
    } catch (std::exception &e) { return std::stoi(e.what()); } catch (...) {
        return -1;
    }
}