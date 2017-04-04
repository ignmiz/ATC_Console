#ifndef ATCABSTRACTPROFILE_H
#define ATCABSTRACTPROFILE_H

#include "atcinterpolator.h"
#include "atcmath.h"

#include <QVector>

class ATCAbstractProfile
{
public:
    explicit ATCAbstractProfile(QVector<double> &levels, QVector<double> &time, QVector<double> &distance, ExType type = ExType::Tangent);
    virtual ~ATCAbstractProfile() = 0;

    void initializeInverseInterpolators(ATCInterpolator &invTime, ATCInterpolator &invDst);

    void setTimeExtrapolationType(ExType t);
    ExType getTimeExtrapolationType();

    void setDistanceExtrapolationType(ExType t);
    ExType getDistanceExtrapolationType();

    double timeInterval(double lvlFrom, double lvlTo);
    double distanceInterval(double lvlFrom, double lvlTo);

    double mixedTimeInterval(double lvlFrom, double interval);
    QVector<double> mixedTimeInterval(double lvlFrom, QVector<double> &intervals);
    double mixedDistanceInterval(double lvlFrom, double interval);
    QVector<double> mixedDistanceInterval(double lvlFrom, QVector<double> &intervals);

private:
    QVector<double> levels;
    QVector<double> time;
    QVector<double> distance;

    ATCInterpolator timeInterpolator;
    ATCInterpolator distanceInterpolator;

    ATCInterpolator inverseTimeInterpolator;
    ATCInterpolator inverseDistanceInterpolator;

    double interval(ATCInterpolator &interpolator, double lvlFrom, double lvlTo);

    double mixedInterval(ATCInterpolator &interpolator, ATCInterpolator &inverseInterpolator, double lvlFrom, double interval);
    QVector<double> mixedInterval(ATCInterpolator &interpolator, ATCInterpolator &inverseInterpolator, double lvlFrom, QVector<double> &intervals);
};

#endif // ATCABSTRACTPROFILE_H
