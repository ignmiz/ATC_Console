
#include "atcprofiledescent.h"

ATCProfileDescent::ATCProfileDescent(QVector<double> &levels, QVector<double> &time, QVector<double> &distance, ExType type) :
    ATCAbstractProfile(levels, time, distance, type)
{
    ATCMath::inverseQVector(levels);
    ATCMath::inverseQVector(time);
    ATCMath::inverseQVector(distance);

    initializeInverseInterpolators(ATCInterpolator(time, levels, type), ATCInterpolator(distance, levels, type));
}

ATCProfileDescent::~ATCProfileDescent()
{

}
