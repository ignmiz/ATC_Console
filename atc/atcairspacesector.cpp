#include "atcairspacesector.h"

ATCAirspaceSector::ATCAirspaceSector()
{

}

ATCAirspaceSector::ATCAirspaceSector(QString name)
{
    sectorName = name;
}

ATCAirspaceSector::~ATCAirspaceSector()
{

}

QString ATCAirspaceSector::getSectorName()
{
    return sectorName;
}

void ATCAirspaceSector::setSectorName(QString name)
{
    sectorName = name;
}


