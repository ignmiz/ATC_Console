#include "atcprocedurestar.h"
#include <QDebug>

ATCProcedureSTAR::ATCProcedureSTAR(QString procedureName, QString airportCode, QString runway) :
    ATCAbstractProcedure(procedureName, airportCode, runway)
{
}

ATCProcedureSTAR::~ATCProcedureSTAR()
{
}
