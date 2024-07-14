#ifndef Q_UNITS
#define Q_UNITS

namespace qrk::units {
//distance
constexpr float m = 1.f;
constexpr float cm = 0.01f;
constexpr float mm = 0.001f;
constexpr float um = 0.000001f;
constexpr float nm = 0.000000001f;
constexpr float km = 1000.f;
constexpr float inch = 0.0254f;
constexpr float foot = 0.3048f;
constexpr float yard = 0.9144f;
constexpr float mile = 1609.34f;
//time
constexpr float s = 1.f;
constexpr float minute = 60.f;
constexpr float hour = 3600.f;
constexpr float day = 86400.f;
constexpr float ms = 0.001f;
constexpr float us = 0.000001f;
constexpr float ns = 0.000000001f;
//energy
constexpr float j = 1.f;
constexpr float kj = 1000.f;
constexpr float Mj = 1000000.f;
constexpr float Gj = 1000000000.f;
constexpr float mj = 0.001f;
constexpr float uj = 0.000001f;
constexpr float nj = 0.000000001f;
constexpr float kcal = 4184.f;
constexpr float cal = 4.184f;
//area
constexpr float cm2 = 0.0001f;
constexpr float m2 = 1.f;
constexpr float km2 = 100000.f;
constexpr float sqInch = 0.00064516f;
constexpr float sqFoot = 0.092903f;
constexpr float sqMile = 2589986.995190719f;
//volume
constexpr float m3 = 1.f;
constexpr float l = 0.001f;
constexpr float cm3 = 0.000001f;
//mass
constexpr float kg = 1.f;
constexpr float g = 0.001f;
constexpr float mg = 0.000001f;
constexpr float ug = 0.000000001f;
constexpr float ng = 0.000000000001f;
constexpr float lb = 0.453592f;
//angle
constexpr float rad = 1.f;
constexpr float deg = 0.0174533f;
//temperature
constexpr float kelvin = 1.f;
inline float ConvertToCelcius(float temp) { return temp - 273.15f; }
inline float ConvertToFarenheit(float temp) { return (temp - 273.15f) * 1.8f + 32.f; }
//work
constexpr float nW = 0.000000001f;
constexpr float uW = 0.000001f;
constexpr float mW = 0.001f;
constexpr float W = 1.f;
constexpr float kW = 1000.f;
constexpr float MW = 1000000.f;
constexpr float GW = 1000000000.f;
constexpr float hp = 745.7f;
//pressure
constexpr float nPa = 0.000000001f;
constexpr float uPa = 0.000001f;
constexpr float mPa = 0.001f;
constexpr float Pa = 1.f;
constexpr float kPa = 1000.f;
constexpr float MPa = 1000000.f;
constexpr float GPa = 1000000000.f;
constexpr float bar = 100000.f;
constexpr float atm = 101325.03982073f;
constexpr float psi = 6894.76f;
//luminescence
constexpr float candela = 1.f;
constexpr float lumen = 0.07955449482f;
}// namespace qrk::units

#endif// !Q_UNITS