#include "atcairspace.h"

ATCAirspace::ATCAirspace()
{

}

ATCAirspace::ATCAirspace(QString name) : airspaceName(name)
{

}

ATCAirspace::~ATCAirspace()
{

}

QString ATCAirspace::getAirspaceName()
{
    return airspaceName;
}

void ATCAirspace::setAirspaceName(QString name)
{
    airspaceName = name;
}
