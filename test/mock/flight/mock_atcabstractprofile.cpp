
#include "mock_atcabstractprofile.h"


Mock_ATCAbstractProfile::Mock_ATCAbstractProfile(QVector<double> &levels, QVector<double> &time, QVector<double> &distance, ExType type) :
    ATCAbstractProfile(levels, time, distance, type)
{

}

Mock_ATCAbstractProfile::~Mock_ATCAbstractProfile()
{

}
