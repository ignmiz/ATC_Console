#include "atcairspace.h"
#include <QDebug>

ATCAirspace::ATCAirspace(QString SCTpath, QString ESEpath)
{
    loadData(SCTpath, ESEpath);
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

ATCSectorARTCCLow* ATCAirspace::findSectorARTCCLow(QString name)
{
    for(int i = 0; i < sectorsARTCCLow.size(); i++)
    {
        if(sectorsARTCCLow.at(i)->getName() == name)
        {
            return sectorsARTCCLow.at(i);
        }
    }

    return nullptr;
}

ATCSectorARTCCHigh* ATCAirspace::findSectorARTCCHigh(QString name)
{
    for(int i = 0; i < sectorsARTCCHigh.size(); i++)
    {
        if(sectorsARTCCHigh.at(i)->getName() == name)
        {
            return sectorsARTCCHigh.at(i);
        }
    }

    return nullptr;
}

ATCSectorARTCC* ATCAirspace::findSectorARTCC(QString name)
{
    for(int i = 0; i < sectorsARTCC.size(); i++)
    {
        if(sectorsARTCC.at(i)->getName() == name)
        {
            return sectorsARTCC.at(i);
        }
    }

    return nullptr;
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

ATCRunwayExtendedCentreline* ATCAirspace::findCentreline(QString airportName, QString rwyID)
{
    for(int i = 0; i < airports.size(); i++)
    {
        if(airports.at(i)->getName() == airportName)
        {
            for(int j = 0; airports.at(i)->getRunwayVectorSize(); j++)
            {
                if(airports.at(i)->getRunway(j)->getRunwayID1() == rwyID)
                {
                    return airports.at(i)->getRunway(j)->getExtendedCentreline1();
                }
                else if(airports.at(i)->getRunway(j)->getRunwayID2() == rwyID)
                {
                    return airports.at(i)->getRunway(j)->getExtendedCentreline2();
                }
            }
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

ATCProcedureSIDSymbol* ATCAirspace::findSIDSymbol(QString name)
{
    for(int i = 0; i < sidSymbols.size(); i++)
    {
        if(sidSymbols.at(i)->getName() == name)
        {
            return sidSymbols.at(i);
        }
    }

    return nullptr;
}

ATCProcedureSTARSymbol* ATCAirspace::findSTARSymbol(QString name)
{
    for(int i = 0; i < starSymbols.size(); i++)
    {
        if(starSymbols.at(i)->getName() == name)
        {
            return starSymbols.at(i);
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

ATCAbstractAirway *ATCAirspace::findAirway(QString name)
{
    for(int i = 0; i < lowAirways.size(); i++)
    {
        if(lowAirways.at(i)->getName() == name)
        {
            return lowAirways.at(i);
        }
    }

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
}

void ATCAirspace::loadData(QString SCTpath, QString ESEpath)
{
//Load sectorfiles from ESE
    QFile eseFile(ESEpath);

    if(!eseFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error while opening ese file...";
        return;
    }

    QTextStream eseStream(&eseFile);
    while(!eseStream.atEnd())
    {
        QString textLine = eseStream.readLine();
        textLine = textLine.trimmed();

        if(textLine.isEmpty() || (textLine.at(0) == ';'))
        {
        }
        else
        {
            if(textLine.contains("SECTORLINE", Qt::CaseInsensitive))
            {
                QStringList stringList = textLine.split(":", QString::SkipEmptyParts);
                QString sectorName = stringList.at(1);

                appendSector(new ATCAirspaceSector(sectorName));
            }
            else if(textLine.contains("COORD", Qt::CaseInsensitive))
            {
                QStringList stringList = textLine.split(":", QString::SkipEmptyParts);
                QString latitudeString = stringList.at(1);
                QString longitudeString = stringList.at(2).left(14);

                double latitudeDouble = coordsStringToDouble(latitudeString);
                double longitudeDouble = coordsStringToDouble(longitudeString);

                getLastSector()->appendAirspaceFix(new ATCAirspaceFix(latitudeDouble, longitudeDouble));
            }
        }
    }

    eseFile.close();

//Load VORs, NDBs, fixes, airports and runways from SCT
    QFile sctFile(SCTpath);

    if(!sctFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening sct file...";
        return;
    }

    bool flagARTCCLow = false;
    bool flagARTCCHigh = false;
    bool flagARTCC = false;
    bool flagVOR = false;
    bool flagNDB = false;
    bool flagFixes = false;
    bool flagAirport = false;
    bool flagRunway = false;
    bool flagSTAR = false;
    bool flagSID = false;
    bool flagLowAirway = false;
    bool flagHighAirway = false;

    ATCSectorARTCCLow *tempARTCCLow;
    ATCSectorARTCCHigh *tempARTCCHigh;
    ATCSectorARTCC *tempARTCC;

    QTextStream sctStream(&sctFile);
    while(!sctStream.atEnd())
    {
        QString textLine = sctStream.readLine();
        textLine = textLine.trimmed();

        if(textLine.isEmpty() || (textLine.at(0) == ';'))
        {
        }
        else
        {
            if(textLine.contains("[ARTCC LOW]", Qt::CaseInsensitive))
            {
                flagARTCCLow = true;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            if(textLine.contains("[ARTCC HIGH]", Qt::CaseInsensitive))
            {
                flagARTCCLow = false;
                flagARTCCHigh = true;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            if(textLine.contains("[ARTCC]", Qt::CaseInsensitive))
            {
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = true;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[VOR]", Qt::CaseInsensitive))
            {
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = true;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[NDB]", Qt::CaseInsensitive))
            {
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = true;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[FIXES]", Qt::CaseInsensitive))
            {
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = true;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[AIRPORT]", Qt::CaseInsensitive))
            {
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = true;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[RUNWAY]", Qt::CaseInsensitive))
            {
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = true;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[STAR]", Qt::CaseInsensitive))
            {
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = true;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[SID]", Qt::CaseInsensitive))
            {
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = true;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[LOW AIRWAY]", Qt::CaseInsensitive))
            {
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = true;
                flagHighAirway = false;
            }
            else if(textLine.contains("[HIGH AIRWAY]", Qt::CaseInsensitive))
            {
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = true;
            }
            else if(flagARTCCLow)
            {
                textLine = textLine.split(";", QString::SkipEmptyParts).at(0);
                textLine = textLine.trimmed();

                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);
                int size = stringList.size();

                int iterator = 0;
                bool coordsFound = false;

                for(int i = 0; i < size - 3; i++)
                {
                    if((isValidCoordsFormat(stringList.at(i)) || isValidNavaid(stringList.at(i))) &&
                            (isValidCoordsFormat(stringList.at(i + 1)) || isValidNavaid(stringList.at(i + 1))) &&
                            (isValidCoordsFormat(stringList.at(i + 2)) || isValidNavaid(stringList.at(i + 2))) &&
                            (isValidCoordsFormat(stringList.at(i + 3)) || isValidNavaid(stringList.at(i + 3))))
                    {
                        iterator = i;
                        coordsFound = true;
                    }
                }

                if(iterator != 0 && coordsFound)
                {
                    QString name(stringList.at(0));

                    for(int i = 1; i < iterator; i++)
                    {
                        name = name + " " + stringList.at(i);
                    }

                    QString lat1String = stringList.at(iterator);
                    QString lon1String = stringList.at(iterator + 1);
                    QString lat2String = stringList.at(iterator + 2);
                    QString lon2String = stringList.at(iterator + 3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(isValidCoordsFormat(lat1String))
                    {
                        lat1 = coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = getNavaidLatitude(lat1String);
                    }

                    if(isValidCoordsFormat(lon1String))
                    {
                        lon1 = coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = getNavaidLongitude(lon1String);
                    }

                    if(isValidCoordsFormat(lat2String))
                    {
                        lat2 = coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = getNavaidLatitude(lat2String);
                    }

                    if(isValidCoordsFormat(lon2String))
                    {
                        lon2 = coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = getNavaidLongitude(lon2String);
                    }

                    ATCSectorARTCCLow *currentObject = findSectorARTCCLow(name);

                    if(currentObject == nullptr)
                    {
                        currentObject = new ATCSectorARTCCLow(name);
                        currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                        currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                        tempARTCCLow = currentObject;

                        appendSectorARTCCLow(currentObject);
                    }
                    else
                    {
                        currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                        currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                        tempARTCCLow = currentObject;
                    }
                }
                else if((iterator == 0) && coordsFound)
                {
                    QString lat1String = stringList.at(0);
                    QString lon1String = stringList.at(1);
                    QString lat2String = stringList.at(2);
                    QString lon2String = stringList.at(3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(isValidCoordsFormat(lat1String))
                    {
                        lat1 = coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = getNavaidLatitude(lat1String);
                    }

                    if(isValidCoordsFormat(lon1String))
                    {
                        lon1 = coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = getNavaidLongitude(lon1String);
                    }

                    if(isValidCoordsFormat(lat2String))
                    {
                        lat2 = coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = getNavaidLatitude(lat2String);
                    }

                    if(isValidCoordsFormat(lon2String))
                    {
                        lon2 = coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = getNavaidLongitude(lon2String);
                    }

                    tempARTCCLow->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    tempARTCCLow->appendCoords2(new ATCAirspaceFix(lat2, lon2));
                }
            }
            else if(flagARTCCHigh)
            {
                textLine = textLine.split(";", QString::SkipEmptyParts).at(0);
                textLine = textLine.trimmed();

                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);
                int size = stringList.size();

                int iterator = 0;
                bool coordsFound = false;

                for(int i = 0; i < size - 3; i++)
                {
                    if((isValidCoordsFormat(stringList.at(i)) || isValidNavaid(stringList.at(i))) &&
                            (isValidCoordsFormat(stringList.at(i + 1)) || isValidNavaid(stringList.at(i + 1))) &&
                            (isValidCoordsFormat(stringList.at(i + 2)) || isValidNavaid(stringList.at(i + 2))) &&
                            (isValidCoordsFormat(stringList.at(i + 3)) || isValidNavaid(stringList.at(i + 3))))
                    {
                        iterator = i;
                        coordsFound = true;
                    }
                }

                if(iterator != 0 && coordsFound)
                {
                    QString name(stringList.at(0));

                    for(int i = 1; i < iterator; i++)
                    {
                        name = name + " " + stringList.at(i);
                    }

                    QString lat1String = stringList.at(iterator);
                    QString lon1String = stringList.at(iterator + 1);
                    QString lat2String = stringList.at(iterator + 2);
                    QString lon2String = stringList.at(iterator + 3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(isValidCoordsFormat(lat1String))
                    {
                        lat1 = coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = getNavaidLatitude(lat1String);
                    }

                    if(isValidCoordsFormat(lon1String))
                    {
                        lon1 = coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = getNavaidLongitude(lon1String);
                    }

                    if(isValidCoordsFormat(lat2String))
                    {
                        lat2 = coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = getNavaidLatitude(lat2String);
                    }

                    if(isValidCoordsFormat(lon2String))
                    {
                        lon2 = coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = getNavaidLongitude(lon2String);
                    }

                    ATCSectorARTCCHigh *currentObject = findSectorARTCCHigh(name);

                    if(currentObject == nullptr)
                    {
                        currentObject = new ATCSectorARTCCHigh(name);
                        currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                        currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                        tempARTCCHigh = currentObject;

                        appendSectorARTCCHigh(currentObject);
                    }
                    else
                    {
                        currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                        currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                        tempARTCCHigh = currentObject;
                    }
                }
                else if((iterator == 0) && coordsFound)
                {
                    QString lat1String = stringList.at(0);
                    QString lon1String = stringList.at(1);
                    QString lat2String = stringList.at(2);
                    QString lon2String = stringList.at(3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(isValidCoordsFormat(lat1String))
                    {
                        lat1 = coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = getNavaidLatitude(lat1String);
                    }

                    if(isValidCoordsFormat(lon1String))
                    {
                        lon1 = coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = getNavaidLongitude(lon1String);
                    }

                    if(isValidCoordsFormat(lat2String))
                    {
                        lat2 = coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = getNavaidLatitude(lat2String);
                    }

                    if(isValidCoordsFormat(lon2String))
                    {
                        lon2 = coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = getNavaidLongitude(lon2String);
                    }

                    tempARTCCHigh->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    tempARTCCHigh->appendCoords2(new ATCAirspaceFix(lat2, lon2));
                }
            }
            else if(flagARTCC)
            {
                textLine = textLine.split(";", QString::SkipEmptyParts).at(0);
                textLine = textLine.trimmed();

                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);
                int size = stringList.size();

                int iterator = 0;
                bool coordsFound = false;

                for(int i = 0; i < size - 3; i++)
                {
                    if((isValidCoordsFormat(stringList.at(i)) || isValidNavaid(stringList.at(i))) &&
                            (isValidCoordsFormat(stringList.at(i + 1)) || isValidNavaid(stringList.at(i + 1))) &&
                            (isValidCoordsFormat(stringList.at(i + 2)) || isValidNavaid(stringList.at(i + 2))) &&
                            (isValidCoordsFormat(stringList.at(i + 3)) || isValidNavaid(stringList.at(i + 3))))
                    {
                        iterator = i;
                        coordsFound = true;
                    }
                }

                if(iterator != 0 && coordsFound)
                {
                    QString name(stringList.at(0));

                    for(int i = 1; i < iterator; i++)
                    {
                        name = name + " " + stringList.at(i);
                    }

                    QString lat1String = stringList.at(iterator);
                    QString lon1String = stringList.at(iterator + 1);
                    QString lat2String = stringList.at(iterator + 2);
                    QString lon2String = stringList.at(iterator + 3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(isValidCoordsFormat(lat1String))
                    {
                        lat1 = coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = getNavaidLatitude(lat1String);
                    }

                    if(isValidCoordsFormat(lon1String))
                    {
                        lon1 = coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = getNavaidLongitude(lon1String);
                    }

                    if(isValidCoordsFormat(lat2String))
                    {
                        lat2 = coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = getNavaidLatitude(lat2String);
                    }

                    if(isValidCoordsFormat(lon2String))
                    {
                        lon2 = coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = getNavaidLongitude(lon2String);
                    }

                    ATCSectorARTCC *currentObject = findSectorARTCC(name);

                    if(currentObject == nullptr)
                    {
                        currentObject = new ATCSectorARTCC(name);
                        currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                        currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                        tempARTCC = currentObject;

                        appendSectorARTCC(currentObject);
                    }
                    else
                    {
                        currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                        currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                        tempARTCC = currentObject;
                    }
                }
                else if((iterator == 0) && coordsFound)
                {
                    QString lat1String = stringList.at(0);
                    QString lon1String = stringList.at(1);
                    QString lat2String = stringList.at(2);
                    QString lon2String = stringList.at(3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(isValidCoordsFormat(lat1String))
                    {
                        lat1 = coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = getNavaidLatitude(lat1String);
                    }

                    if(isValidCoordsFormat(lon1String))
                    {
                        lon1 = coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = getNavaidLongitude(lon1String);
                    }

                    if(isValidCoordsFormat(lat2String))
                    {
                        lat2 = coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = getNavaidLatitude(lat2String);
                    }

                    if(isValidCoordsFormat(lon2String))
                    {
                        lon2 = coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = getNavaidLongitude(lon2String);
                    }

                    tempARTCC->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    tempARTCC->appendCoords2(new ATCAirspaceFix(lat2, lon2));
                }
            }
            else if(flagFixes)
            {
                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);

                QString fixName = stringList.at(0);
                QString latitudeString = stringList.at(1);
                QString longitudeString = stringList.at(2).left(14);

                double latitudeDouble = coordsStringToDouble(latitudeString);
                double longitudeDouble = coordsStringToDouble(longitudeString);

                appendFix(new ATCNavFix(fixName, latitudeDouble, longitudeDouble));
            }
            else if(flagVOR)
            {
                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);

                QString vorName = stringList.at(0);
                float frequency = stringList.at(1).toFloat();
                QString latitudeString = stringList.at(2);
                QString longitudeString = stringList.at(3).left(14);

                double latitudeDouble = coordsStringToDouble(latitudeString);
                double longitudeDouble = coordsStringToDouble(longitudeString);

                appendVOR(new ATCBeaconVOR(vorName, frequency, latitudeDouble, longitudeDouble));
            }
            else if(flagNDB)
            {
                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);

                QString ndbName = stringList.at(0);
                float frequency = stringList.at(1).toFloat();
                QString latitudeString = stringList.at(2);
                QString longitudeString = stringList.at(3).left(14);

                double latitudeDouble = coordsStringToDouble(latitudeString);
                double longitudeDouble = coordsStringToDouble(longitudeString);

                appendNDB(new ATCBeaconNDB(ndbName, frequency, latitudeDouble, longitudeDouble));
            }
            else if(flagAirport)
            {
                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);

                QString airportName = stringList.at(0);
                QString latitudeString = stringList.at(2);
                QString longitudeString = stringList.at(3).left(14);

                double latitudeDouble = coordsStringToDouble(latitudeString);
                double longitudeDouble = coordsStringToDouble(longitudeString);

                appendAirport(new ATCAirport(airportName, latitudeDouble, longitudeDouble));
            }
            else if(flagRunway)
            {
                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);

                QString rwyID1 = stringList.at(0);
                QString rwyID2 = stringList.at(1);
                unsigned int magneticHDG1 = stringList.at(2).toUInt();
                unsigned int magneticHDG2 = stringList.at(3).toUInt();
                double startLat = coordsStringToDouble(stringList.at(4));
                double startLon = coordsStringToDouble(stringList.at(5));
                double endLat = coordsStringToDouble(stringList.at(6));
                double endLon = coordsStringToDouble(stringList.at(7).left(14));
                QString airportName = stringList.at(8).left(4);

                ATCAirport *desiredAirport = findAirport(airportName);

                if(desiredAirport != nullptr)
                {
                    desiredAirport->appendRunway(new ATCRunway(rwyID1, rwyID2, magneticHDG1, magneticHDG2, startLat, startLon, endLat, endLon));
                }
            }
            else if(flagSTAR)
            {
                textLine = textLine.split(";", QString::SkipEmptyParts).at(0);
                textLine = textLine.trimmed();

                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);
                int size = stringList.size();

                int iterator = 0;
                bool coordsFound = false;

                for(int i = 0; i < size - 3; i++)
                {
                    if((isValidCoordsFormat(stringList.at(i)) || isValidNavaid(stringList.at(i))) &&
                            (isValidCoordsFormat(stringList.at(i + 1)) || isValidNavaid(stringList.at(i + 1))) &&
                            (isValidCoordsFormat(stringList.at(i + 2)) || isValidNavaid(stringList.at(i + 2))) &&
                            (isValidCoordsFormat(stringList.at(i + 3)) || isValidNavaid(stringList.at(i + 3))))
                    {
                        iterator = i;
                        coordsFound = true;
                    }
                }

                if(iterator != 0 && coordsFound)
                {
                    QString name(stringList.at(0));

                    for(int i = 1; i < iterator; i++)
                    {
                        name = name + " " + stringList.at(i);
                    }

                    QString lat1String = stringList.at(iterator);
                    QString lon1String = stringList.at(iterator + 1);
                    QString lat2String = stringList.at(iterator + 2);
                    QString lon2String = stringList.at(iterator + 3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(isValidCoordsFormat(lat1String))
                    {
                        lat1 = coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = getNavaidLatitude(lat1String);
                    }

                    if(isValidCoordsFormat(lon1String))
                    {
                        lon1 = coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = getNavaidLongitude(lon1String);
                    }

                    if(isValidCoordsFormat(lat2String))
                    {
                        lat2 = coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = getNavaidLatitude(lat2String);
                    }

                    if(isValidCoordsFormat(lon2String))
                    {
                        lon2 = coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = getNavaidLongitude(lon2String);
                    }

                    ATCProcedureSTARSymbol *currentObject = new ATCProcedureSTARSymbol(name);
                    currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                    appendSTARSymbol(currentObject);
                }
                else if((iterator == 0) && coordsFound)
                {
                    QString lat1String = stringList.at(0);
                    QString lon1String = stringList.at(1);
                    QString lat2String = stringList.at(2);
                    QString lon2String = stringList.at(3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(isValidCoordsFormat(lat1String))
                    {
                        lat1 = coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = getNavaidLatitude(lat1String);
                    }

                    if(isValidCoordsFormat(lon1String))
                    {
                        lon1 = coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = getNavaidLongitude(lon1String);
                    }

                    if(isValidCoordsFormat(lat2String))
                    {
                        lat2 = coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = getNavaidLatitude(lat2String);
                    }

                    if(isValidCoordsFormat(lon2String))
                    {
                        lon2 = coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = getNavaidLongitude(lon2String);
                    }

                    getLastSTARSymbol()->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    getLastSTARSymbol()->appendCoords2(new ATCAirspaceFix(lat2, lon2));
                }
            }
            else if(flagSID)
            {
                textLine = textLine.split(";", QString::SkipEmptyParts).at(0);
                textLine = textLine.trimmed();

                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);
                int size = stringList.size();

                int iterator = 0;
                bool coordsFound = false;

                for(int i = 0; i < size - 3; i++)
                {
                    if((isValidCoordsFormat(stringList.at(i)) || isValidNavaid(stringList.at(i))) &&
                            (isValidCoordsFormat(stringList.at(i + 1)) || isValidNavaid(stringList.at(i + 1))) &&
                            (isValidCoordsFormat(stringList.at(i + 2)) || isValidNavaid(stringList.at(i + 2))) &&
                            (isValidCoordsFormat(stringList.at(i + 3)) || isValidNavaid(stringList.at(i + 3))))
                    {
                        iterator = i;
                        coordsFound = true;
                    }
                }

                if(iterator != 0 && coordsFound)
                {
                    QString name(stringList.at(0));

                    for(int i = 1; i < iterator; i++)
                    {
                        name = name + " " + stringList.at(i);
                    }

                    QString lat1String = stringList.at(iterator);
                    QString lon1String = stringList.at(iterator + 1);
                    QString lat2String = stringList.at(iterator + 2);
                    QString lon2String = stringList.at(iterator + 3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(isValidCoordsFormat(lat1String))
                    {
                        lat1 = coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = getNavaidLatitude(lat1String);
                    }

                    if(isValidCoordsFormat(lon1String))
                    {
                        lon1 = coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = getNavaidLongitude(lon1String);
                    }

                    if(isValidCoordsFormat(lat2String))
                    {
                        lat2 = coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = getNavaidLatitude(lat2String);
                    }

                    if(isValidCoordsFormat(lon2String))
                    {
                        lon2 = coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = getNavaidLongitude(lon2String);
                    }

                    ATCProcedureSIDSymbol *currentObject = new ATCProcedureSIDSymbol(name);
                    currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                    appendSIDSymbol(currentObject);
                }
                else if((iterator == 0) && coordsFound)
                {
                    QString lat1String = stringList.at(0);
                    QString lon1String = stringList.at(1);
                    QString lat2String = stringList.at(2);
                    QString lon2String = stringList.at(3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(isValidCoordsFormat(lat1String))
                    {
                        lat1 = coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = getNavaidLatitude(lat1String);
                    }

                    if(isValidCoordsFormat(lon1String))
                    {
                        lon1 = coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = getNavaidLongitude(lon1String);
                    }

                    if(isValidCoordsFormat(lat2String))
                    {
                        lat2 = coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = getNavaidLatitude(lat2String);
                    }

                    if(isValidCoordsFormat(lon2String))
                    {
                        lon2 = coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = getNavaidLongitude(lon2String);
                    }

                    getLastSIDSymbol()->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    getLastSIDSymbol()->appendCoords2(new ATCAirspaceFix(lat2, lon2));
                }
            }
            else if(flagLowAirway)
            {
                textLine = textLine.split(";", QString::SkipEmptyParts).at(0);
                textLine = textLine.trimmed();

                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);
                int size = stringList.size();

                int iterator = 0;
                bool coordsFound = false;

                for(int i = 0; i < size - 3; i++)
                {
                    if((isValidCoordsFormat(stringList.at(i)) || isValidNavaid(stringList.at(i))) &&
                            (isValidCoordsFormat(stringList.at(i + 1)) || isValidNavaid(stringList.at(i + 1))) &&
                            (isValidCoordsFormat(stringList.at(i + 2)) || isValidNavaid(stringList.at(i + 2))) &&
                            (isValidCoordsFormat(stringList.at(i + 3)) || isValidNavaid(stringList.at(i + 3))))
                    {
                        iterator = i;
                        coordsFound = true;
                    }
                }

                if(iterator != 0 && coordsFound)
                {
                    QString name(stringList.at(0));

                    for(int i = 1; i < iterator; i++)
                    {
                        name = name + " " + stringList.at(i);
                    }

                    QString lat1String = stringList.at(iterator);
                    QString lon1String = stringList.at(iterator + 1);
                    QString lat2String = stringList.at(iterator + 2);
                    QString lon2String = stringList.at(iterator + 3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(isValidCoordsFormat(lat1String))
                    {
                        lat1 = coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = getNavaidLatitude(lat1String);
                    }

                    if(isValidCoordsFormat(lon1String))
                    {
                        lon1 = coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = getNavaidLongitude(lon1String);
                    }

                    if(isValidCoordsFormat(lat2String))
                    {
                        lat2 = coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = getNavaidLatitude(lat2String);
                    }

                    if(isValidCoordsFormat(lon2String))
                    {
                        lon2 = coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = getNavaidLongitude(lon2String);
                    }

                    if(isAirwayLow(name))
                    {
                        ATCAirwayLow *currentObject = findAirwayLow(name);
                        currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                        currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                        currentObject->appendRouteFix(lat1String);
                        currentObject->appendRouteFix(lat2String);
                        currentObject->getRouteFixes().removeDuplicates();
                    }
                    else
                    {
                        ATCAirwayLow *currentObject = new ATCAirwayLow(name);
                        currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                        currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                        currentObject->appendRouteFix(lat1String);
                        currentObject->appendRouteFix(lat2String);
                        currentObject->getRouteFixes().removeDuplicates();

                        appendAirwayLow(currentObject);
                    }
                }
            }
            else if(flagHighAirway)
            {
                textLine = textLine.split(";", QString::SkipEmptyParts).at(0);
                textLine = textLine.trimmed();

                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);
                int size = stringList.size();

                int iterator = 0;
                bool coordsFound = false;

                for(int i = 0; i < size - 3; i++)
                {
                    if((isValidCoordsFormat(stringList.at(i)) || isValidNavaid(stringList.at(i))) &&
                            (isValidCoordsFormat(stringList.at(i + 1)) || isValidNavaid(stringList.at(i + 1))) &&
                            (isValidCoordsFormat(stringList.at(i + 2)) || isValidNavaid(stringList.at(i + 2))) &&
                            (isValidCoordsFormat(stringList.at(i + 3)) || isValidNavaid(stringList.at(i + 3))))
                    {
                        iterator = i;
                        coordsFound = true;
                    }
                }

                if(iterator != 0 && coordsFound)
                {
                    QString name(stringList.at(0));

                    for(int i = 1; i < iterator; i++)
                    {
                        name = name + " " + stringList.at(i);
                    }

                    QString lat1String = stringList.at(iterator);
                    QString lon1String = stringList.at(iterator + 1);
                    QString lat2String = stringList.at(iterator + 2);
                    QString lon2String = stringList.at(iterator + 3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(isValidCoordsFormat(lat1String))
                    {
                        lat1 = coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = getNavaidLatitude(lat1String);
                    }

                    if(isValidCoordsFormat(lon1String))
                    {
                        lon1 = coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = getNavaidLongitude(lon1String);
                    }

                    if(isValidCoordsFormat(lat2String))
                    {
                        lat2 = coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = getNavaidLatitude(lat2String);
                    }

                    if(isValidCoordsFormat(lon2String))
                    {
                        lon2 = coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = getNavaidLongitude(lon2String);
                    }

                    if(isAirwayHigh(name))
                    {
                        ATCAirwayHigh *currentObject = findAirwayHigh(name);
                        currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                        currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                        currentObject->appendRouteFix(lat1String);
                        currentObject->appendRouteFix(lat2String);
                        currentObject->getRouteFixes().removeDuplicates();
                    }
                    else
                    {
                        ATCAirwayHigh *currentObject = new ATCAirwayHigh(name);
                        currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                        currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                        currentObject->appendRouteFix(lat1String);
                        currentObject->appendRouteFix(lat2String);
                        currentObject->getRouteFixes().removeDuplicates();

                        appendAirwayHigh(currentObject);
                    }
                }
            }
        }
    }

    sctFile.close();

//Load SID and STAR procedures from ESE
    if(!eseFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error while opening ese file...";
        return;
    }

    bool flagSidStar = false;
    bool flagAirspace = false;

    QTextStream eseStream2(&eseFile);
    while(!eseStream2.atEnd())
    {
        QString textLine = eseStream2.readLine();
        textLine = textLine.trimmed();

        if(textLine.isEmpty() || (textLine.at(0) == ';'))
        {
        }
        else
        {
            if(textLine.contains("[SIDSSTARS]", Qt::CaseInsensitive))
            {
                flagSidStar = true;
                flagAirspace = false;
            }
            else if(textLine.contains("[AIRSPACE]", Qt::CaseInsensitive))
            {
                flagSidStar = false;
                flagAirspace = true;
            }
            else if(flagSidStar)
            {
                QStringList stringList = textLine.split(":", QString::SkipEmptyParts);

                QString procedureType = stringList.at(0);
                QString airportCode = stringList.at(1);
                QString runwayID = stringList.at(2);
                QString procedureName = stringList.at(3);
                QString fixListString = stringList.at(4);

                QRegExp expression("(\\s|\\t)");
                QStringList fixList = fixListString.split(expression, QString::SkipEmptyParts);

                if(procedureType == "SID")
                {
                    ATCProcedureSID *currentProcedure = new ATCProcedureSID(procedureName, airportCode, runwayID);
                    currentProcedure->setFixList(fixList);

                    appendSID(currentProcedure);
                }
                else if(procedureType == "STAR")
                {
                    ATCProcedureSTAR *currentProcedure = new ATCProcedureSTAR(procedureName, airportCode, runwayID);
                    currentProcedure->setFixList(fixList);

                    appendSTAR(currentProcedure);
                }
            }
        }
    }

    eseFile.close();
}
