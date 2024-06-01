#ifndef QRK_COLOR
#define QRK_COLOR

#include <inttypes.h>
#include <algorithm>

namespace qrk
{
	struct Color
	{
		uint8_t r, g, b, a;
	};
	struct ColorF
	{
		float r, g, b, a;
	};

	inline Color ConvertTo8Bit(ColorF color)
	{
		return Color(
			std::clamp((uint8_t)color.r * 255, 0, 255),
			std::clamp((uint8_t)color.g * 255, 0, 255),
			std::clamp((uint8_t)color.b * 255, 0, 255),
			std::clamp((uint8_t)color.a * 255, 0, 255));
	}
	inline ColorF ConvertToFloat(Color color)
	{
		return ColorF(
			std::clamp((float)color.r / 255, 0.f, 1.f),
			std::clamp((float)color.g / 255, 0.f, 1.f),
			std::clamp((float)color.b / 255, 0.f, 1.f),
			std::clamp((float)color.a / 255, 0.f, 1.f));
	}
}

#endif // !QRK_COLOR
