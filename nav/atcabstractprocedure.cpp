#include "atcabstractprocedure.h"

ATCAbstractProcedure::ATCAbstractProcedure(QString procedureName, QString airportCode, QString runway) :
    name(procedureName),
    airport(airportCode),
    runwayID(runway)
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

QString ATCAbstractProcedure::getFixName(int iterator)
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

void ATCAbstractProcedure::setFixList(QStringList &list)
{
    fixList = list;
}

void ATCAbstractProcedure::appendFixName(QString fixName)
{
    fixList.append(fixName);
}

void ATCAbstractProcedure::show()
{
    visible = true;
}

void ATCAbstractProcedure::hide()
{
    visible = false;
}

bool ATCAbstractProcedure::isVisible()
{
    return visible;
}

void ATCAbstractProcedure::setFlagVisible(bool flag)
{
    visible = flag;
}
