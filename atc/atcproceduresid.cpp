#include "atcproceduresid.h"

ATCProcedureSID::ATCProcedureSID(QString procedureName, QString airportCode, QString runway) :
    ATCAbstractProcedure(procedureName, airportCode, runway)
{
}

ATCProcedureSID::~ATCProcedureSID()
{

}
