#include "atcprocedurestarsymbol.h"
#include <QDebug>

ATCProcedureSTARSymbol::ATCProcedureSTARSymbol(QString name) : ATCAbstractProcedureSymbol(name)
{

}

ATCProcedureSTARSymbol::~ATCProcedureSTARSymbol()
{
    qDebug() << "STAR symbol: " << getName() << " deleted...";
}
