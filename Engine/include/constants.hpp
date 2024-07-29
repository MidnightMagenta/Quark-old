#ifndef Q_CONSTANTS
#define Q_CONSTANTS

#include "../include/units.hpp"

namespace qrk::constants {
constexpr float PI = 3.14159265f;
constexpr float TAU = 6.28318530f;
constexpr float c = 299792458.f;

inline float RadToDeg(float angle) { return (angle * 180) / PI; }
inline float DegToRad(float angle) { return (angle * PI) / 180; }
}// namespace qrk::constants

#endif// !Q_CONSTANTS