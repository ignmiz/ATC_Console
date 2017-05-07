#ifndef ATCABSTRACTPROFILE_H
#define ATCABSTRACTPROFILE_H

#include "atcinterpolator2d.h"
#include "atcmath.h"

#include <QVector>

class ATCAbstractProfile
{
public:
    explicit ATCAbstractProfile();
    virtual ~ATCAbstractProfile() = 0;

    ATCInterpolator2D* getTimeInterpolator();
    ATCInterpolator2D* getDistanceInterpolator();

    ATCInterpolator2D* getInverseTimeInterpolator();
    ATCInterpolator2D* getInverseDistanceInterpolator();

    double timeInterval(double key, double lvlFrom, double lvlTo);
    double distanceInterval(double key, double lvlFrom, double lvlTo);

    double mixedTimeInterval(double key, double lvlFrom, double interval);
    QVector<double> mixedTimeInterval(double key, double lvlFrom, QVector<double> &intervals);
    double mixedDistanceInterval(double key, double lvlFrom, double interval);
    QVector<double> mixedDistanceInterval(double key, double lvlFrom, QVector<double> &intervals);

private:
    ATCInterpolator2D timeInterpolator;
    ATCInterpolator2D distanceInterpolator;

    ATCInterpolator2D inverseTimeInterpolator;
    ATCInterpolator2D inverseDistanceInterpolator;

    double interval(ATCInterpolator2D &interpolator, double key, double lvlFrom, double lvlTo);

    double mixedInterval(ATCInterpolator2D &interpolator, ATCInterpolator2D &inverseInterpolator, double key, double lvlFrom, double interval);
    QVector<double> mixedInterval(ATCInterpolator2D &interpolator, ATCInterpolator2D &inverseInterpolator, double key, double lvlFrom, QVector<double> &intervals);
};

#endif // ATCABSTRACTPROFILE_H
