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
    deleteAllNDBs();
    deleteAllAirports();
    deleteAllSIDs();
    deleteAllSTARs();
}

double ATCAirspace::coordsStringToDouble(QString coords)
{
    QString hemisphereID = coords.at(0);

    coords = coords.remove(0, 1);
    QStringList stringList = coords.split(".", QString::SkipEmptyParts);

    double degrees = stringList.at(0).toDouble();
    double minutes = stringList.at(1).toDouble();
    double seconds = stringList.at(2).toDouble() + stringList.at(3).toDouble() / 1000;

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

void ATCAirspace::appendNDB(ATCBeaconNDB *ndb)
{
    ndbs.append(ndb);
}

void ATCAirspace::appendAirport(ATCAirport *airport)
{
    airports.append(airport);
}

void ATCAirspace::appendSID(ATCProcedureSID *sid)
{
    sids.append(sid);
}

void ATCAirspace::appendSTAR(ATCProcedureSTAR *star)
{
    stars.append(star);
}

ATCAirspaceSector* ATCAirspace::getSector(int iterator)
{
    return sectors.at(iterator);
}

ATCAirspaceSector* ATCAirspace::getLastSector()
{
    return sectors[sectors.size() - 1];
}

int ATCAirspace::getSectorVectorSize()
{
    return sectors.size();
}

int ATCAirspace::getFixesVectorSize()
{
    return fixes.size();
}

int ATCAirspace::getVORsVectorSize()
{
    return vors.size();
}

int ATCAirspace::getNDBsVectorSize()
{
    return ndbs.size();
}

int ATCAirspace::getAirportsVectorSize()
{
    return airports.size();
}

int ATCAirspace::getSIDsVectorSize()
{
    return sids.size();
}

int ATCAirspace::getSTARsVectorSize()
{
    return stars.size();
}

ATCNavFix* ATCAirspace::getFix(int iterator)
{
    return fixes.at(iterator);
}

ATCBeaconVOR* ATCAirspace::getVOR(int iterator)
{
    return vors.at(iterator);
}

ATCBeaconNDB* ATCAirspace::getNDB(int iterator)
{
    return ndbs.at(iterator);
}

ATCAirport* ATCAirspace::getAirport(int iterator)
{
    return airports.at(iterator);
}

ATCProcedureSID* ATCAirspace::getSID(int iterator)
{
    return sids.at(iterator);
}

ATCProcedureSTAR* ATCAirspace::getSTAR(int iterator)
{
    return stars.at(iterator);
}

ATCAirport* ATCAirspace::findAirport(QString ICAOname)
{
    for(int i = 0; i < airports.size(); i++)
    {
        if(airports.at(i)->getName() == ICAOname)
        {
            return airports.at(i);
        }
    }

    return nullptr;
}

ATCNavFix *ATCAirspace::findFix(QString fixName)
{
    for(int i = 0; i < fixes.size(); i++)
    {
        if(fixes.at(i)->getName() == fixName)
        {
            return fixes.at(i);
        }
    }

    return nullptr;
}

void ATCAirspace::deleteAllSectors()
{
    if(!sectors.empty())
    {
        for(int i = 0; i < sectors.size(); i++)
        {
            delete sectors.at(i);
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
            delete fixes.at(i);
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
            delete vors.at(i);
        }
    }
    else
    {
        qDebug() << "Empty vector of VORs...";
    }
}

void ATCAirspace::deleteAllNDBs()
{
    if(!ndbs.empty())
    {
        for(int i = 0; i < ndbs.size(); i++)
        {
            delete ndbs.at(i);
        }
    }
    else
    {
        qDebug() << "Empty vector of NDBs...";
    }
}

void ATCAirspace::deleteAllAirports()
{
    if(!airports.empty())
    {
        for(int i = 0; i < airports.size(); i++)
        {
            delete airports.at(i);
        }
    }
    else
    {
        qDebug() << "Empty vector of airports...";
    }
}

void ATCAirspace::deleteAllSIDs()
{
    if(!sids.empty())
    {
        for(int i = 0; i < sids.size(); i++)
        {
            delete sids.at(i);
        }
    }
    else
    {
        qDebug() << "Empty vector of SIDs";
    }
}

void ATCAirspace::deleteAllSTARs()
{
    if(!stars.empty())
    {
        for(int i = 0; i < stars.size(); i++)
        {
            delete stars.at(i);
        }
    }
    else
    {
        qDebug() << "Empty vector of STARs";
    }
}
