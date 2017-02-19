#ifndef ATCCONST_H
#define ATCCONST_H

#include <QtGlobal>

class ATCConst
{
public:
    ATCConst();

    //General
    static const double PI;

    //Conversion (depreciated)
    static const double DEG_2_RAD;
    static const double RAD_2_DEG;

    static const double NM_2_M;

    //Magnetism & Ellipsoid
    static const double AVG_DECLINATION;

    static const double WGS84_A;
    static const double WGS84_B;
    static const double WGS84_AVG_RADIUS;
    static const double WGS84_FIRST_ECCENTRICITY;
    static const double WGS84_INVERSE_FLATTENING;

    //Scene & Display
    static const int DISPLAY_WIDTH;
    static const int DISPLAY_HEIGHT;

    static const int SCENE_FACTOR;
    static const int SCENE_WIDTH;
    static const int SCENE_HEIGHT;
    static const double SECTOR_SHRINK_FACTOR;

    //Simulation Performance
    static const double DT;
    static const double REFRESH_INTERVAL;

    static const double k1;
    static const double k2;

    static const double FLY_OVER_DST;
    static const double APP_RANGE;
    static const double APP_ALLOWED_XTRACK_INTERCEPT;
    static const double APP_ALLOWED_HDG_INTERCEPT;

    //Environment
    static const double betaTrop;
    static const double betaStrat1;
    static const double betaStrat2;
    static const double betaMeso1;
    static const double betaMeso2;

    static const double kappa;
    static const double R;

    static const double g0;

    //BADA GPF Parameters Extract
    static const double ACC_LONG_MAX;
    static const double ACC_NORM_MAX;

    static const double NOM_BANK_ANGLE;

    static const double C_DES_EXP;

    static const double C_TH_TO;
    static const double C_TH_CR;

    static const double C_V_MIN;
    static const double C_V_MIN_TO;

    static const double V_CL1;
    static const double V_CL2;
    static const double V_CL3;
    static const double V_CL4;
    static const double V_CL5;
    static const double V_CL6;
    static const double V_CL7;
    static const double V_CL8;

    static const double V_DES1;
    static const double V_DES2;
    static const double V_DES3;
    static const double V_DES4;
    static const double V_DES5;
    static const double V_DES6;
    static const double V_DES7;

    static const double C_RED_PISTON;
    static const double C_RED_TURBO;
    static const double C_RED_JET;
};

#endif // ATCCONST_H
