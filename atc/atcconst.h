#ifndef ATCCONST_H
#define ATCCONST_H

class ATCConst
{
public:
    ATCConst();

    static const double PI;

    static const double DEG_2_RAD;
    static const double RAD_2_DEG;

    static const double NM_2_M;

    static const double AVG_DECLINATION;

    static const double WGS84_RADIUS;
    static const double WGS84_FIRST_ECCENTRICITY;

    static const int DISPLAY_WIDTH = 1920;
    static const int DISPLAY_HEIGHT = 1020;

    static const int SCENE_FACTOR = 100;
    static const int SCENE_WIDTH = DISPLAY_WIDTH * SCENE_FACTOR;
    static const int SCENE_HEIGHT = DISPLAY_HEIGHT * SCENE_FACTOR;
    static const double SECTOR_SHRINK_FACTOR;
};

#endif // ATCCONST_H
