#include "atcnavfix.h"

ATCNavFix::ATCNavFix(double latitude, double longitude)
{

}

ATCNavFix::~ATCNavFix()
{

}

QString ATCNavFix::getName()
{
    return name;
}

void ATCNavFix::setName(QString fixName)
{
    name = fixName;
}
