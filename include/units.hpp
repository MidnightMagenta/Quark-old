#ifndef Q_UNITS
#define Q_UNITS

namespace qrk::units {
//distance
const float m = 1.f;
const float cm = 0.01f;
const float mm = 0.001f;
const float um = 0.000001f;
const float nm = 0.000000001f;
const float km = 1000.f;
const float inch = 0.0254f;
const float foot = 0.3048f;
const float yard = 0.9144f;
const float mile = 1609.34f;
//time
const float s = 1.f;
const float minute = 60.f;
const float hour = 3600.f;
const float day = 86400.f;
const float ms = 0.001f;
const float us = 0.000001f;
const float ns = 0.000000001f;
//energy
const float j = 1.f;
const float kj = 1000.f;
const float Mj = 1000000.f;
const float Gj = 1000000000.f;
const float mj = 0.001f;
const float uj = 0.000001f;
const float nj = 0.000000001f;
const float kcal = 4184.f;
const float cal = 4.184f;
//area
const float cm2 = 0.0001f;
const float m2 = 1.f;
const float km2 = 100000.f;
const float sqInch = 0.00064516f;
const float sqFoot = 0.092903f;
const float sqMile = 2589986.995190719f;
//volume
const float m3 = 1.f;
const float l = 0.001f;
const float cm3 = 0.000001f;
//mass
const float kg = 1.f;
const float g = 0.001f;
const float mg = 0.000001f;
const float ug = 0.000000001f;
const float ng = 0.000000000001f;
const float lb = 453.592f;
//angle
const float rad = 1.f;
const float deg = 0.0174533f;
//temperature
const float kelvin = 1.f;
inline float ConvertToCelcius(float temp) { return temp - 273.15f; }
inline float ConvertToFarenheit(float temp) { return (temp - 273.15f) * 1.8f + 32.f; }
//work
const float nW = 0.000000001f;
const float uW = 0.000001f;
const float mW = 0.001f;
const float W = 1.f;
const float kW = 1000.f;
const float MW = 1000000.f;
const float GW = 1000000000.f;
const float hp = 745.7f;
//pressure
const float nPa = 0.000000001f;
const float uPa = 0.000001f;
const float mPa = 0.001f;
const float Pa = 1.f;
const float kPa = 1000.f;
const float MPa = 1000000.f;
const float GPa = 1000000000.f;
const float bar = 100000.f;
const float atm = 101325.03982073f;
const float psi = 6894.76f;
//luminescence
const float candela = 1.f;
const float lumen = 0.07955449482f;
}// namespace qrk::units

#endif// !Q_UNITS