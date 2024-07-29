#ifndef Q_RENDER_WINDOW
#define Q_RENDER_WINDOW

#include "../include/draw.hpp"
#include "../include/window.hpp"

namespace qrk {
struct RenderWindowSettings {
    qrk::WindowSettings windowSettings = {Q_WINDOW_DEFAULT,
                                          8,
                                          {255, 255, 255, 255},
                                          4,
                                          6};
    qrk::RendererSettings renderSettings = {true, true, true, true};
};

class RenderWindow {
public:
    RenderWindow(qrk::vec2u windowSize, std::string windowName,
                 qrk::RenderWindowSettings settings =
                         {{Q_WINDOW_DEFAULT, 8, {255, 255, 255, 255}, 4, 6},
                          {true, true, true, true}})
        : window(windowName, windowSize, settings.windowSettings),
          renderer(window, settings.renderSettings) {
        window.MakeContextCurrent();
    }

    void ClearWindow() { window.Clear(); }
    qrk::glWindow &GetWindow() { return window; }
    qrk::qb_GL_Renderer &GetRenderer() { return renderer; }

    template<drawDataStruct draw_t>
    void QueueDraw(const draw_t &drawData, bool UI = false) {
        renderer.QueueDraw(drawData, UI);
    }

    void Draw() {
        renderer.Draw();
        window.SwapWindowBuffers();
    }

    void Close() { window.Close(); }
    bool IsOpen() { return window.IsOpen(); }

private:
    qrk::glWindow window;
    qrk::qb_GL_Renderer renderer;
};
}// namespace qrk

#endif// !Q_RENDER_WINDOW
