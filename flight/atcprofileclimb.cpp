
#include "atcprofileclimb.h"

ATCProfileClimb::ATCProfileClimb(QVector<double> &levels, QVector<double> &time, QVector<double> &distance, ExType type) :
    ATCAbstractProfile(levels, time, distance, type)
{
    initializeInverseInterpolators(ATCInterpolator(time, levels, type), ATCInterpolator(distance, levels, type));
}

ATCProfileClimb::~ATCProfileClimb()
{

}
