
#include "atcprofiledescent.h"

ATCProfileDescent::ATCProfileDescent(QVector<double> &levels, QVector<double> &time, QVector<double> &distance, ExType type) :
    ATCAbstractProfile(levels, time, distance, type)
{

}

ATCProfileDescent::~ATCProfileDescent()
{

}

