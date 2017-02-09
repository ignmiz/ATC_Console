#include "atcconst.h"

ATCConst::ATCConst()
{

}

//General
const double ATCConst::PI = 3.14159265359;

//Conversion (depreciated)
const double ATCConst::DEG_2_RAD = ATCConst::PI / 180;
const double ATCConst::RAD_2_DEG = 180 / ATCConst::PI;

const double ATCConst::NM_2_M = 1852;

//Magnetism & Ellipsoid
const double ATCConst::AVG_DECLINATION = 5;

const double ATCConst::WGS84_A = 6378137;
const double ATCConst::WGS84_B = 6356752.314245;
const double ATCConst::WGS84_AVG_RADIUS = 6367435.679716;
const double ATCConst::WGS84_FIRST_ECCENTRICITY = 0.08181919092890624;
const double ATCConst::WGS84_INVERSE_FLATTENING = 298.257223563;

//Scene & Display
const int ATCConst::DISPLAY_WIDTH = 1920;
const int ATCConst::DISPLAY_HEIGHT = 1020;

const int ATCConst::SCENE_FACTOR = 100;
const int ATCConst::SCENE_WIDTH = ATCConst::DISPLAY_WIDTH * ATCConst::SCENE_FACTOR;
const int ATCConst::SCENE_HEIGHT = ATCConst::DISPLAY_HEIGHT * ATCConst::SCENE_FACTOR;
const double ATCConst::SECTOR_SHRINK_FACTOR = 2;

//Simulation Performance
const double ATCConst::DT = 0.1;
const double ATCConst::REFRESH_INTERVAL = 3;

const double ATCConst::k1 = 0.005;
const double ATCConst::k2 = -33;

const double ATCConst::FLY_OVER_DST = 185.2;

//Environment
const double ATCConst::betaTrop = -0.0065;
const double ATCConst::betaStrat1 = 0.001;
const double ATCConst::betaStrat2 = 0.0028;
const double ATCConst::betaMeso1 = -0.0028;
const double ATCConst::betaMeso2 = -0.002;

const double ATCConst::kappa = 1.4;
const double ATCConst::R = 287.05287;

const double ATCConst::g0 = 9.80665;

//BADA GPF Parameters Extract
const double ATCConst::ACC_LONG_MAX = 2;    //ftps^2
const double ATCConst::ACC_NORM_MAX = 5;    //ftps^2

//const double ATCConst::NOM_BANK_ANGLE = ATCMath::deg2rad(30);
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
