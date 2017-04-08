
#include "atcprofilespeed.h"

ATCProfileSpeed::ATCProfileSpeed(QVector<double> &lvls, QVector<double> &climbSpds, QVector<double> &crsSpds, QVector<double> &dscSpds, ExType type) :
    levels(lvls),
    climbSpeeds(climbSpds),
    cruiseSpeeds(crsSpds),
    descentSpeeds(dscSpds),
    climbInterpolator(levels, climbSpds, type),
    cruiseInterpolator(levels, crsSpds, type),
    descentInterpolator(levels, dscSpds, type)
{

}

ATCProfileSpeed::~ATCProfileSpeed()
{

}

double ATCProfileSpeed::nominalClimbSpeed(double lvl)
{
    return interpolate(climbInterpolator, lvl);
}

double ATCProfileSpeed::nominalCruiseSpeed(double lvl)
{
    return interpolate(cruiseInterpolator, lvl);
}

double ATCProfileSpeed::nominalDescentSpeed(double lvl)
{
    return interpolate(descentInterpolator, lvl);
}

double ATCProfileSpeed::interpolate(ATCInterpolator &interpolator, double lvl)
{
    QVector<double> site(1, lvl);
    QVector<double> result(1);

    interpolator.interpolate(site, result);
    return result.at(0);
}
