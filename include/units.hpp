#ifndef Q_UNITS
#define Q_UNITS

namespace qrk::units
{
	//distance
	const float m = 1;
	const float cm = 0.01;
	const float mm = 0.001;
	const float um = 0.000001;
	const float nm = 0.000000001;
	const float km = 1000;
	const float inch = 0.0254;
	const float foot = 0.3048;
	const float yard = 0.9144;
	const float mile = 1609.34;
	//time
	const float s = 1;
	const float minute = 60;
	const float hour = 3600;
	const float day = 86400;
	const float ms = 0.001;
	const float us = 0.000001;
	const float ns = 0.000000001;
	//energy
	const float j = 1;
	const float kj = 1000;
	const float mj = 1000000;
	const float gj = 1000000000;
	const float mj = 0.001;
	const float uj = 0.000001;
	const float nj = 0.000000001;
	const float kcal = 4184;
	const float cal = 4.184;
	//area
	const float m2 = 1;
	const float km2 = 100000;
	const float sqInch = 0.00064516;
	const float sqFoot = 0.092903;
	const float sqMile = 2589986.995190719;
	//volume 
	const float m3 = 1;
	const float l = 0.001;
	const float cm2 = 0.000001;
	//mass
	const float kg = 1;
	const float g = 0.001;
	const float mg = 0.000001;
	const float ug = 0.000000001;
	const float ng = 0.000000000001;
	const float lb = 453.592;
}

#endif // !Q_UNITS
