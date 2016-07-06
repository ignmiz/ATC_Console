#include "atcabstractprocedure.h"

ATCAbstractProcedure::ATCAbstractProcedure(QString procedureName) : name(procedureName)
{
}

ATCAbstractProcedure::~ATCAbstractProcedure()
{
}

QString ATCAbstractProcedure::getName()
{
    return name;
}

QString ATCAbstractProcedure::getAirport()
{
    return airport;
}

QString ATCAbstractProcedure::getRunwayID()
{
    return runwayID;
}

ATCNavFix* ATCAbstractProcedure::getFix(int iterator)
{
    return fixList.at(iterator);
}

int ATCAbstractProcedure::getFixListSize()
{
    return fixList.size();
}

void ATCAbstractProcedure::setName(QString procedureName)
{
    name = procedureName;
}

void ATCAbstractProcedure::setAirport(QString airportCode)
{
    airport = airportCode;
}

void ATCAbstractProcedure::setRunwayID(QString runway)
{
    runwayID = runway;
}

void ATCAbstractProcedure::appendFix(ATCNavFix *fix)
{
    fixList.append(fix);
}
