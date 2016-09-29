#ifndef ATCPROCEDURESIDSYMBOL_H
#define ATCPROCEDURESIDSYMBOL_H

#include "atcabstractproceduresymbol.h"

class ATCProcedureSIDSymbol : public ATCAbstractProcedureSymbol
{
public:
    ATCProcedureSIDSymbol(QString name);
    ~ATCProcedureSIDSymbol();
};

#endif // ATCPROCEDURESIDSYMBOL_H
