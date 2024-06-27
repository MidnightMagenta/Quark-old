#ifndef QRK_COLOR
#define QRK_COLOR

#include <algorithm>
#include <inttypes.h>

//#define Q_COLOR_WHITE qrk::Color(255, 255, 255, 255)
//#define Q_COLOR_BLACK qrk::Color(0, 0, 0, 255)
//#define Q_COLOR_RED qrk::Color(255, 0, 0, 255)
//#define Q_COLOR_GREEN qrk::Color(0, 255, 0, 255)
//#define Q_COLOR_BLUE qrk::Color(0, 0, 255, 255)
//#define Q_COLOR_MIDNIGHT_BLUE qrk::Color(0, 49, 83, 255)

namespace qrk {
struct Color {
    uint8_t r, g, b, a;
};
struct ColorF {
    float r, g, b, a;
};

inline Color ConvertTo8Bit(ColorF color) {
    return Color(std::clamp((uint8_t) color.r * 255, 0, 255),
                 std::clamp((uint8_t) color.g * 255, 0, 255),
                 std::clamp((uint8_t) color.b * 255, 0, 255),
                 std::clamp((uint8_t) color.a * 255, 0, 255));
}
inline ColorF ConvertToFloat(Color color) {
    return ColorF(std::clamp((float) color.r / 255, 0.f, 1.f),
                  std::clamp((float) color.g / 255, 0.f, 1.f),
                  std::clamp((float) color.b / 255, 0.f, 1.f),
                  std::clamp((float) color.a / 255, 0.f, 1.f));
}

namespace Q_COLOR {
constexpr qrk::Color WHITE = {255, 255, 255, 255};
constexpr qrk::Color RED = {255, 0, 0, 255};
constexpr qrk::Color GREEN = {0, 255, 0, 255};
constexpr qrk::Color BLUE = {0, 0, 255, 255};
constexpr qrk::Color BLACK = {0, 0, 0, 255};
constexpr qrk::Color MIDNIGHT_BLUE = {0, 49, 83, 255};
}// namespace Q_COLOR
}// namespace qrk

#endif// !QRK_COLOR
