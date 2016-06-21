#include "atcconst.h"

ATCConst::ATCConst()
{

}

const double ATCConst::PI = 3.14159265359;

const double ATCConst::DEG_2_RAD = ATCConst::PI / 180;
const double ATCConst::RAD_2_DEG = 180 / ATCConst::PI;

const double ATCConst::WGS84_RADIUS = 6378137;
const double ATCConst::WGS84_FIRST_ECCENTRICITY = 0.08181919092890624;
