#ifndef ATCPROCEDURESID_H
#define ATCPROCEDURESID_H

#include "atcabstractprocedure.h"

class ATCProcedureSID : public ATCAbstractProcedure
{
public:
    explicit ATCProcedureSID(QString procedureName, QString airportCode, QString runway);
    ~ATCProcedureSID();
};

#endif // ATCPROCEDURESID_H
