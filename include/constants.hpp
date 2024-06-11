#ifndef Q_CONSTANTS
#define Q_CONSTANTS

#include "../include/units.hpp"

namespace qrk::constants {
inline const float PI = 3.14159265358979323846;
inline const float TAU = 6.28318530717958647692;
inline const float c = 300000000;

inline float RadToDeg(float angle) { return (angle * 180) / PI; }
inline float DegToRad(float angle) { return (angle * PI) / 180; }
}// namespace qrk::constants

#endif// !Q_CONSTANTS