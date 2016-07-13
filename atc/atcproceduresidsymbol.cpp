#include "atcproceduresidsymbol.h"

ATCProcedureSIDSymbol::ATCProcedureSIDSymbol(QString name) : ATCAbstractProcedureSymbol(name)
{

}

ATCProcedureSIDSymbol::~ATCProcedureSIDSymbol()
{
    qDebug() << "SID symbol: " << getName() << " deleted...";
}
