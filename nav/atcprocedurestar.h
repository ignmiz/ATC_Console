#ifndef ATCPROCEDURESTAR_H
#define ATCPROCEDURESTAR_H

#include "atcabstractprocedure.h"

class ATCProcedureSTAR : public ATCAbstractProcedure
{
public:
    ATCProcedureSTAR(QString procedureName, QString airportCode, QString runway);
    ~ATCProcedureSTAR();
};

#endif // ATCPROCEDURESTAR_H
