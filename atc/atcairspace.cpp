#include "atcairspace.h"
#include <QDebug>

ATCAirspace::ATCAirspace()
{

}

ATCAirspace::~ATCAirspace()
{
    deleteAllSectors();
    deleteAllFixes();
    deleteAllVORs();
}

double ATCAirspace::coordsStringToDouble(QString coords)
{
    QString hemisphereID = coords.at(0);

    coords = coords.remove(0, 1);
    QStringList stringList = coords.split(".", QString::SkipEmptyParts);

    double degrees = stringList[0].toDouble();
    double minutes = stringList[1].toDouble();
    double seconds = stringList[2].toDouble() + stringList[3].toDouble() / 1000;

    double coordsDouble = degrees + minutes / 60 + seconds / 3600;

    if((hemisphereID == "N") || (hemisphereID == "E"))
    {
        return coordsDouble;
    }
    else
    {
        return (-1) * coordsDouble;
    }
}

void ATCAirspace::appendSector(ATCAirspaceSector *sector)
{
    sectors.append(sector);
}

void ATCAirspace::appendFix(ATCNavFix *fix)
{
    fixes.append(fix);
}

void ATCAirspace::appendVOR(ATCBeaconVOR *vor)
{
    vors.append(vor);
}

ATCAirspaceSector* ATCAirspace::getSector(int iterator)
{
    return sectors[iterator];
}

ATCAirspaceSector* ATCAirspace::getLastSector()
{
    return sectors[sectors.size() - 1];
}

int ATCAirspace::getSectorVectorSize()
{
    return sectors.size();
}

ATCNavFix* ATCAirspace::getFix(int iterator)
{
    return fixes[iterator];
}

void ATCAirspace::deleteAllSectors()
{
    if(!sectors.empty())
    {
        for(int i = 0; i < sectors.size(); i++)
        {
            delete sectors[i];
        }
    }
    else
    {
        qDebug() << "Empty vector of sectors...";
    }
}

void ATCAirspace::deleteAllFixes()
{
    if(!fixes.empty())
    {
        for(int i = 0; i < fixes.size(); i++)
        {
            delete fixes[i];
        }
    }
    else
    {
        qDebug() << "Empty vector of fixes...";
    }
}

void ATCAirspace::deleteAllVORs()
{
    if(!vors.empty())
    {
        for(int i = 0; i < vors.size(); i++)
        {
            delete vors[i];
        }
    }
    else
    {
        qDebug() << "Empty vector of VORs...";
    }
}
