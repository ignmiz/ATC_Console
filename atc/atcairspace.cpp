#include "atcairspace.h"
#include <QDebug>

ATCAirspace::ATCAirspace()
{

}

ATCAirspace::~ATCAirspace()
{
    deleteAllSectors();
    deleteAllSectorsARTCCLow();
    deleteAllSectorsARTCCHigh();
    deleteAllSectorsARTCC();
    deleteAllFixes();
    deleteAllVORs();
    deleteAllNDBs();
    deleteAllAirports();
    deleteAllSIDs();
    deleteAllSTARs();
    deleteAllSIDSymbols();
    deleteAllSTARSymbols();
    deleteAllAirwayLow();
    deleteAllAirwayHigh();
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

    if((hemisphereID == "N") || (hemisphereID == "E") || (hemisphereID == "n") || (hemisphereID == "e"))
    {
        return coordsDouble;
    }
    else
    {
        return (-1) * coordsDouble;
    }
}

bool ATCAirspace::isValidCoordsFormat(QString coordString)
{
    if((coordString.size() == 14) && ((coordString.at(0) == "N") || (coordString.at(0) == "n") ||
                                      (coordString.at(0) == "S") || (coordString.at(0) == "s") ||
                                      (coordString.at(0) == "E") || (coordString.at(0) == "e") ||
                                      (coordString.at(0) == "W") || (coordString.at(0) == "w"))
            && (coordString.at(4) == ".") && (coordString.at(7) == ".") && (coordString.at(10) == "."))
    {
        return true;
    }
    else return false;
}

bool ATCAirspace::isValidNavaid(QString name)
{
    if(isFix(name))
    {
        return true;
    }
    else if(isVOR(name))
    {
        return true;
    }
    else if(isNDB(name))
    {
        return true;
    }
    else if(isAirport(name))
    {
        return true;
    }
    else return false;
}

bool ATCAirspace::isFix(QString name)
{
    for(int i = 0; i < fixes.size(); i++)
    {
        if(fixes.at(i)->getName() == name) return true;
    }

    return false;
}

bool ATCAirspace::isVOR(QString name)
{
    for(int i = 0; i < vors.size(); i++)
    {
        if(vors.at(i)->getName() == name) return true;
    }

    return false;
}

bool ATCAirspace::isNDB(QString name)
{
    for(int i = 0; i < ndbs.size(); i++)
    {
        if(ndbs.at(i)->getName() == name) return true;
    }

    return false;
}

bool ATCAirspace::isAirport(QString name)
{
    for(int i = 0; i < airports.size(); i++)
    {
        if(airports.at(i)->getName() == name) return true;
    }

    return false;
}

bool ATCAirspace::isAirwayLow(QString name)
{
    for(int i = 0; i < lowAirways.size(); i++)
    {
        if(lowAirways.at(i)->getName() == name) return true;
    }

    return false;
}

bool ATCAirspace::isAirwayHigh(QString name)
{
    for(int i = 0; i < highAirways.size(); i++)
    {
        if(highAirways.at(i)->getName() == name) return true;
    }

    return false;
}

void ATCAirspace::appendSector(ATCAirspaceSector *sector)
{
    sectors.append(sector);
}

void ATCAirspace::appendSectorARTCCLow(ATCSectorARTCCLow *sector)
{
    sectorsARTCCLow.append(sector);
}

void ATCAirspace::appendSectorARTCCHigh(ATCSectorARTCCHigh *sector)
{
    sectorsARTCCHigh.append(sector);
}

void ATCAirspace::appendSectorARTCC(ATCSectorARTCC *sector)
{
    sectorsARTCC.append(sector);
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

void ATCAirspace::appendSIDSymbol(ATCProcedureSIDSymbol *symbol)
{
    sidSymbols.append(symbol);
}

void ATCAirspace::appendSTARSymbol(ATCProcedureSTARSymbol *symbol)
{
    starSymbols.append(symbol);
}

void ATCAirspace::appendAirwayLow(ATCAirwayLow *airway)
{
    lowAirways.append(airway);
}

void ATCAirspace::appendAirwayHigh(ATCAirwayHigh *airway)
{
    highAirways.append(airway);
}

ATCAirspaceSector* ATCAirspace::getSector(int iterator)
{
    return sectors.at(iterator);
}

ATCSectorARTCCLow* ATCAirspace::getSectorARTCCLow(int iterator)
{
    return sectorsARTCCLow.at(iterator);
}

ATCSectorARTCCHigh* ATCAirspace::getSectorARTCCHigh(int iterator)
{
    return sectorsARTCCHigh.at(iterator);
}

ATCSectorARTCC* ATCAirspace::getSectorARTCC(int iterator)
{
    return sectorsARTCC.at(iterator);
}

ATCAirspaceSector* ATCAirspace::getLastSector()
{
    return sectors.at(sectors.size() - 1);
}

ATCSectorARTCCLow* ATCAirspace::getLastSectorARTCCLow()
{
    return sectorsARTCCLow.at(sectorsARTCCLow.size() - 1);
}

ATCSectorARTCCHigh* ATCAirspace::getLastSectorARTCCHigh()
{
    return sectorsARTCCHigh.at(sectorsARTCCHigh.size() - 1);
}

ATCSectorARTCC* ATCAirspace::getLastSectorARTCC()
{
    return sectorsARTCC.at(sectorsARTCC.size() - 1);
}

ATCProcedureSIDSymbol* ATCAirspace::getLastSIDSymbol()
{
    return sidSymbols.at(sidSymbols.size() - 1);
}

ATCProcedureSTARSymbol* ATCAirspace::getLastSTARSymbol()
{
    return starSymbols.at(starSymbols.size() - 1);
}

ATCAirwayLow* ATCAirspace::getLastAirwayLow()
{
    return lowAirways.at(lowAirways.size() - 1);
}

ATCAirwayHigh* ATCAirspace::getLastAirwayHigh()
{
    return highAirways.at(highAirways.size() - 1);
}

int ATCAirspace::getSectorVectorSize()
{
    return sectors.size();
}

int ATCAirspace::getSectorARTCCLowVectorSize()
{
    return sectorsARTCCLow.size();
}

int ATCAirspace::getSectorARTCCHighVectorSize()
{
    return sectorsARTCCHigh.size();
}

int ATCAirspace::getSectorARTCCVectorSize()
{
    return sectorsARTCC.size();
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

int ATCAirspace::getSIDSymbolsVectorSize()
{
    return sidSymbols.size();
}

int ATCAirspace::getSTARSymbolsVectorSize()
{
    return starSymbols.size();
}

int ATCAirspace::getAirwayLowVectorSize()
{
    return lowAirways.size();
}

int ATCAirspace::getAirwayHighVectorSize()
{
    return highAirways.size();
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

ATCProcedureSIDSymbol* ATCAirspace::getSIDSymbol(int iterator)
{
    return sidSymbols.at(iterator);
}

ATCProcedureSTARSymbol* ATCAirspace::getSTARSymbol(int iterator)
{
    return starSymbols.at(iterator);
}

ATCAirwayLow* ATCAirspace::getAirwayLow(int iterator)
{
    return lowAirways.at(iterator);
}

ATCAirwayHigh *ATCAirspace::getAirwayHigh(int iterator)
{
    return highAirways.at(iterator);
}

QVector<ATCSectorARTCCLow*>& ATCAirspace::getSectorARTCCLowVector()
{
    return sectorsARTCCLow;
}

QVector<ATCSectorARTCCHigh*>& ATCAirspace::getSectorARTCCHighVector()
{
    return sectorsARTCCHigh;
}

QVector<ATCSectorARTCC*>& ATCAirspace::getSectorARTCCVector()
{
    return sectorsARTCC;
}

QVector<ATCNavFix*>& ATCAirspace::getFixesVector()
{
    return fixes;
}

QVector<ATCBeaconNDB*>& ATCAirspace::getNDBsVector()
{
    return ndbs;
}

QVector<ATCBeaconVOR*>& ATCAirspace::getVORsVector()
{
    return vors;
}

QVector<ATCAirport*>& ATCAirspace::getAirportsVector()
{
    return airports;
}

QVector<ATCProcedureSIDSymbol*>& ATCAirspace::getSIDSymbolsVector()
{
    return sidSymbols;
}

QVector<ATCProcedureSTARSymbol*>& ATCAirspace::getSTARSymbolsVector()
{
    return starSymbols;
}

QVector<ATCAirwayLow*>& ATCAirspace::getAirwayLowVector()
{
    return lowAirways;
}

QVector<ATCAirwayHigh*>& ATCAirspace::getAirwayHighVector()
{
    return highAirways;
}

double ATCAirspace::getNavaidLatitude(QString name)
{
    if(isFix(name))
    {
        return findFix(name)->latitude();
    }
    else if(isVOR(name))
    {
        return findVOR(name)->latitude();
    }
    else if(isNDB(name))
    {
        return findNDB(name)->latitude();
    }
    else if(isAirport(name))
    {
        return findAirport(name)->latitude();
    }

    return -200;
}

double ATCAirspace::getNavaidLongitude(QString name)
{
    if(isFix(name))
    {
        return findFix(name)->longitude();
    }
    else if(isVOR(name))
    {
        return findVOR(name)->longitude();
    }
    else if(isNDB(name))
    {
        return findNDB(name)->longitude();
    }
    else if(isAirport(name))
    {
        return findAirport(name)->longitude();
    }

    return -200;
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

ATCBeaconVOR *ATCAirspace::findVOR(QString name)
{
    for(int i = 0; i < vors.size(); i++)
    {
        if(vors.at(i)->getName() == name)
        {
            return vors.at(i);
        }
    }

    return nullptr;
}

ATCBeaconNDB *ATCAirspace::findNDB(QString name)
{
    for(int i = 0; i < ndbs.size(); i++)
    {
        if(ndbs.at(i)->getName() == name)
        {
            return ndbs.at(i);
        }
    }

    return nullptr;
}

ATCAirwayLow *ATCAirspace::findAirwayLow(QString name)
{
    for(int i = 0; i < lowAirways.size(); i++)
    {
        if(lowAirways.at(i)->getName() == name)
        {
            return lowAirways.at(i);
        }
    }

    return nullptr;
}

ATCAirwayHigh *ATCAirspace::findAirwayHigh(QString name)
{
    for(int i = 0; i < highAirways.size(); i++)
    {
        if(highAirways.at(i)->getName() == name)
        {
            return highAirways.at(i);
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
        qDebug() << "Empty vector of Sectors...";
    }
}

void ATCAirspace::deleteAllSectorsARTCCLow()
{
    if(!sectorsARTCCLow.empty())
    {
        for(int i = 0; i < sectorsARTCCLow.size(); i++)
        {
            delete sectorsARTCCLow.at(i);
        }
    }
    else
    {
        qDebug() << "Empty vector of Sectors ARTCC Low...";
    }
}

void ATCAirspace::deleteAllSectorsARTCCHigh()
{
    if(!sectorsARTCCHigh.empty())
    {
        for(int i = 0; i < sectorsARTCCHigh.size(); i++)
        {
            delete sectorsARTCCHigh.at(i);
        }
    }
    else
    {
        qDebug() << "Empty vector of Sectors ARTCC High...";
    }
}

void ATCAirspace::deleteAllSectorsARTCC()
{
    if(!sectorsARTCC.empty())
    {
        for(int i = 0; i < sectorsARTCC.size(); i++)
        {
            delete sectorsARTCC.at(i);
        }
    }
    else
    {
        qDebug() << "Empty vector of Sectors ARTCC...";
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

void ATCAirspace::deleteAllSIDSymbols()
{
    if(!sidSymbols.empty())
    {
        for(int i = 0; i < sidSymbols.size(); i++)
        {
            delete sidSymbols.at(i);
        }
    }
    else
    {
        qDebug() << "Empty vector of SID symbols...";
    }
}

void ATCAirspace::deleteAllSTARSymbols()
{
    if(!starSymbols.empty())
    {
        for(int i = 0; i < starSymbols.size(); i++)
        {
            delete starSymbols.at(i);
        }
    }
    else
    {
        qDebug() << "Empty vector of STAR symbols...";
    }
}

void ATCAirspace::deleteAllAirwayLow()
{
    if(!lowAirways.empty())
    {
        for(int i = 0; i < lowAirways.size(); i++)
        {
            delete lowAirways.at(i);
        }
    }
    else
    {
        qDebug() << "Empty vector of low airways...";
    }
}

void ATCAirspace::deleteAllAirwayHigh()
{
    if(!highAirways.empty())
    {
        for(int i = 0; i < highAirways.size(); i++)
        {
            delete highAirways.at(i);
        }
    }
    else
    {
        qDebug() << "Empty vector of high airways...";
    }
}
