#include "STB/stb_truetype.h"


#include <../include/event.hpp>
#include <../include/glyph_renderer.hpp>
#include <../include/object.hpp>
#include <../include/rect.hpp>
#include <../include/render_window.hpp>
#include <../include/misc_functions.hpp>

//define entry point of the application
int run() {
    qrk::RenderWindowSettings rws;
    rws.windowSettings.clearColor = {10, 10, 10, 255};
    qrk::RenderWindow window(qrk::vec2u({800, 800}), "TestWindow", rws);
    qrk::Texture2D texture("resources/textures/grid.png");
    qrk::Texture2D texture_w("resources/textures/white.jpg");
    qrk::Event e(window.GetWindow());

    qrk::Object obj("resources/objects/cube.obj", false);
    qrk::GLObject gl_obj(obj);
    gl_obj.SetPosition(0, 0, -10);
    gl_obj.SetTexture(texture);
    gl_obj.SetRotation(45, 45, 45);

    qrk::Rect rect;
    rect.SetSize(window.GetWindow().GetSize().x(), window.GetWindow().GetSize().y());
    rect.SetPosition(0, 0);
    rect.SetOffset(-rect.GetSize().x() / 2, -rect.GetSize().y() / 2);

    qrk::Font fnt("resources/fonts/ariblk.ttf", 60, 500);
    qrk::Text fpsText(fnt);
    rect.SetTexture(texture);
    qrk::debug::FrameCounter fc;
    while (window.IsOpen()) {
        e.UpdateWindow();
        if (e.KeyDown(qrk::ESCAPE)) { window.Close(); }
        window.ClearWindow();
        fpsText.SetText("FPS: " + qrk::misc::to_string_precision(fc.GetFrameRate(), 2));
        if(window.IsOpen()){ fpsText.SetPosition(-static_cast<float>(window.GetWindow().GetSize().x()) / 2 + 10, -static_cast<float>(window.GetWindow().GetSize().y()) / 2); }
        window.QueueDraw(rect.GetDrawData());
        //window.QueueDraw(gl_obj.GetDrawData());
        window.QueueDraw(fpsText.GetDrawData());
        window.Draw();
    }
    return 1;
}

int main() {
    try {
        return run();
    } catch (std::exception &e) { return std::stoi(e.what()); } catch (...) {
        qrk::debug::LogError("Unhandled exception");
        return -1;
    }
}