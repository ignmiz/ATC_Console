#include "atcabstractproceduresymbol.h"

ATCAbstractProcedureSymbol::ATCAbstractProcedureSymbol(QString name) : symbolName(name)
{
}

ATCAbstractProcedureSymbol::~ATCAbstractProcedureSymbol()
{
    if(!lines.empty())
    {
        for(int i = 0; i < lines.size(); i++)
        {
            delete lines.at(i);
        }
    }
}

QString ATCAbstractProcedureSymbol::getName()
{
    return symbolName;
}

QGraphicsLineItem *ATCAbstractProcedureSymbol::getLine(int iterator)
{
    return lines.at(iterator);
}

void ATCAbstractProcedureSymbol::setName(QString name)
{
    symbolName = name;
}

void ATCAbstractProcedureSymbol::appendLine(QGraphicsLineItem *line)
{
    lines.append(line);
}
