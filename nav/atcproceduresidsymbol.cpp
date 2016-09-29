#include "atcproceduresidsymbol.h"
#include <QDebug>

ATCProcedureSIDSymbol::ATCProcedureSIDSymbol(QString name) : ATCAbstractProcedureSymbol(name)
{

}

ATCProcedureSIDSymbol::~ATCProcedureSIDSymbol()
{
    qDebug() << "SID symbol: " << getName() << " deleted...";
}
