#include "atcconst.h"

ATCConst::ATCConst()
{

}

const double ATCConst::PI = 3.14159265359;

const double ATCConst::DEG_2_RAD = ATCConst::PI / 180;
const double ATCConst::RAD_2_DEG = 180 / ATCConst::PI;

const double ATCConst::NM_2_M = 1852;

const double ATCConst::AVG_DECLINATION = 5;

const double ATCConst::WGS84_RADIUS = 6378137;
const double ATCConst::WGS84_FIRST_ECCENTRICITY = 0.08181919092890624;

const double ATCConst::SECTOR_SHRINK_FACTOR = 2;

const double ATCConst::ACC_LONG_MAX = 2;    //ftps^2
const double ATCConst::ACC_NORM_MAX = 5;    //ftps^2

const double ATCConst::NOM_BANK_ANGLE = 30;

const double ATCConst::C_DES_EXP = 1.6;

const double ATCConst::C_TH_TO = 1.2;
const double ATCConst::C_TH_CR = 0.95;

const double ATCConst::C_V_MIN = 1.3;
const double ATCConst::C_V_MIN_TO = 1.2;

const double ATCConst::V_CL1 = 5;
const double ATCConst::V_CL2 = 10;
const double ATCConst::V_CL3 = 30;
const double ATCConst::V_CL4 = 60;
const double ATCConst::V_CL5 = 80;
const double ATCConst::V_CL6 = 20;
const double ATCConst::V_CL7 = 30;
const double ATCConst::V_CL8 = 35;

const double ATCConst::V_DES1 = 5;
const double ATCConst::V_DES2 = 10;
const double ATCConst::V_DES3 = 20;
const double ATCConst::V_DES4 = 50;
const double ATCConst::V_DES5 = 5;
const double ATCConst::V_DES6 = 10;
const double ATCConst::V_DES7 = 20;

const double ATCConst::C_RED_PISTON = 0;
const double ATCConst::C_RED_TURBO = 0.25;
const double ATCConst::C_RED_JET = 0.15;
