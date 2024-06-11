#ifndef Q_CONSTANTS
#define Q_CONSTANTS

#include "../include/units.hpp"

namespace qrk::constants
{
	const float PI = 3.14159265358979323846;
	const float TAU = 6.28318530717958647692;
	const float c = 300000000;

	inline float RadToDeg(float angle) { return (angle * 180) / PI; }
	inline float DegToRad(float angle) { return (angle * PI) / 180; }
}

#endif // !Q_CONSTANTS