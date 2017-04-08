#ifndef ATCPROFILESPEED_H
#define ATCPROFILESPEED_H

#include "atcinterpolator.h"
#include "atcmath.h"

#include <QVector>

class ATCProfileSpeed
{
public:
    explicit ATCProfileSpeed(QVector<double> &lvls, QVector<double> &climbSpds, QVector<double> &crsSpds, QVector<double> &dscSpds, ExType type = ExType::Saturation);
    ~ATCProfileSpeed();

    double nominalClimbSpeed(double lvl);
    double nominalCruiseSpeed(double lvl);
    double nominalDescentSpeed(double lvl);

private:
    QVector<double> levels;
    QVector<double> climbSpeeds;
    QVector<double> cruiseSpeeds;
    QVector<double> descentSpeeds;

    ATCInterpolator climbInterpolator;
    ATCInterpolator cruiseInterpolator;
    ATCInterpolator descentInterpolator;

    double interpolate(ATCInterpolator &interpolator, double lvl);
};

#endif // ATCPROFILESPEED_H
