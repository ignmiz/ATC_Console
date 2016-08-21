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

    if(!coords1.empty())
    {
        for(int i = 0; i < coords1.size(); i++)
        {
            delete coords1.at(i);
        }
    }

    if(!coords2.empty())
    {
        for(int i = 0; i < coords2.size(); i++)
        {
            delete coords2.at(i);
        }
    }
}

QString ATCAbstractProcedureSymbol::getName()
{
    return symbolName;
}

ATCAirspaceFix *ATCAbstractProcedureSymbol::getCoords1(int iterator)
{
    return coords1.at(iterator);
}

ATCAirspaceFix *ATCAbstractProcedureSymbol::getCoords2(int iterator)
{
    return coords2.at(iterator);
}

QGraphicsLineItem *ATCAbstractProcedureSymbol::getLine(int iterator)
{
    return lines.at(iterator);
}

int ATCAbstractProcedureSymbol::getCoordsVectorSize()
{
    return coords1.size();
}

void ATCAbstractProcedureSymbol::setName(QString name)
{
    symbolName = name;
}

void ATCAbstractProcedureSymbol::appendCoords1(ATCAirspaceFix *coord)
{
    coords1.append(coord);
}

void ATCAbstractProcedureSymbol::appendCoords2(ATCAirspaceFix *coord)
{
    coords2.append(coord);
}

void ATCAbstractProcedureSymbol::appendLine(QGraphicsLineItem *line)
{
    lines.append(line);
}

void ATCAbstractProcedureSymbol::show()
{
    for(int i = 0; i < lines.size(); i++)
    {
        lines.at(i)->show();
    }

    visible = true;
}

void ATCAbstractProcedureSymbol::hide()
{
    for(int i = 0; i < lines.size(); i++)
    {
        lines.at(i)->hide();
    }

    visible = false;
}

bool ATCAbstractProcedureSymbol::isVisible()
{
    return visible;
}

void ATCAbstractProcedureSymbol::setFlagVisible(bool flag)
{
    visible = flag;
}
