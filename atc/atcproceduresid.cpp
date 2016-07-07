#include "atcproceduresid.h"
#include <QDebug>

ATCProcedureSID::ATCProcedureSID(QString procedureName, QString airportCode, QString runway) :
    ATCAbstractProcedure(procedureName, airportCode, runway)
{
}

ATCProcedureSID::~ATCProcedureSID()
{
    qDebug() << "SID: " << getAirport() << ":" << getRunwayID() << ":" << getName() << " deleted...";
}
