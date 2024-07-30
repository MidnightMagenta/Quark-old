#include <../include/event.hpp>
#include <../include/glyph_renderer.hpp>
#include <../include/misc_functions.hpp>
#include <../include/object.hpp>
#include <../include/rect.hpp>
#include <../include/render_window.hpp>

//define entry point of the application
int run() {
    qrk::RenderWindowSettings rws;
    rws.windowSettings.clearColor = {10, 10, 10, 255};
    qrk::RenderWindow window(qrk::vec2u({800, 800}), "TestWindow", rws);
    qrk::Texture2D texture("resources/textures/testTexture.png");
    qrk::Event e(window.GetWindow());

    qrk::Object obj("resources/objects/smooth_uv_sphere.obj");

    qrk::Rect rect;
    rect.SetSize(window.GetWindow().GetSize().x(),
                 window.GetWindow().GetSize().y());
    rect.SetPosition(0, 0);
    rect.SetOffset(-rect.GetSize().x() / 2, -rect.GetSize().y() / 2);

    qrk::Font fnt("resources/fonts/ariblk.ttf", 60, 500);
    qrk::Text fpsText(fnt);
    rect.SetTexture(texture);
    qrk::debug::FrameCounter fc;

    while (obj.WaitForLoad(window.GetWindow()) && window.IsOpen()) {
        window.ClearWindow();
        fpsText.SetText("FPS: " +
                        qrk::misc::to_string_precision(fc.GetFrameRate(), 2));
        fpsText.SetPosition(
                -static_cast<float>(window.GetWindow().GetSize().x()) / 2 + 10,
                -static_cast<float>(window.GetWindow().GetSize().y()) / 2);
        window.QueueDraw(fpsText.GetDrawData());
        window.Draw();
    }

    qrk::GLObject gl_obj(obj);
    qrk::LightSource ls(qrk::vec3f({30, 30, 0}), {255, 255, 255, 255});
    window.GetRenderer().AddLightSource(ls);
    gl_obj.SetPosition(0, 0, -10);
    gl_obj.SetTexture(texture);

    while (window.IsOpen()) {
        e.UpdateWindow();
        if (e.KeyDown(qrk::ESCAPE)) { window.Close(); }
        window.ClearWindow();
        fpsText.SetText("FPS: " +
                        qrk::misc::to_string_precision(fc.GetFrameRate(), 2));
        fpsText.SetPosition(
                -static_cast<float>(window.GetWindow().GetSize().x()) / 2 + 10,
                -static_cast<float>(window.GetWindow().GetSize().y()) / 2);
        window.QueueDraw(rect.GetDrawData());
        window.QueueDraw(gl_obj.GetDrawData());
        window.QueueDraw(fpsText.GetDrawData());
        window.Draw();
    }
    return 1;
}

#ifdef SUBSYSTEM_CONSOLE
int main(){
    try {
        return run();
    } catch (std::exception &e) {
        try {
            return std::stoi(e.what());
        } catch (...) {
            const std::string error(e.what());
            qrk::debug::ShowErrorBox(error);
            qrk::debug::LogError(error);
            return -1;
        }
    } catch (...) {
        qrk::debug::LogError("Unhandled exception");
        return -1;
    }
}
#elif SUBSYSTEM_WINDOWS
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    try {
        return run();
    } catch (std::exception &e) {
        try {
            return std::stoi(e.what());
        } catch (...) {
            const std::string error(e.what());
            qrk::debug::ShowErrorBox(error);
            qrk::debug::LogError(error);
            return -1;
        }
    } catch (...) {
        qrk::debug::LogError("Unhandled exception");
        return -1;
    }
}
#endif