#ifndef ATCAIRSPACE_H
#define ATCAIRSPACE_H

#include "atcairspacesector.h"
#include "atcnavfix.h"
#include "atcbeaconvor.h"
#include "atcbeaconndb.h"
#include "atcairport.h"
#include "atcproceduresid.h"
#include "atcprocedurestar.h"
#include "atcproceduresidsymbol.h"
#include "atcprocedurestarsymbol.h"

#include <QString>
#include <QVector>

class ATCAirspace
{

public:
    explicit ATCAirspace();
    ~ATCAirspace();

    double coordsStringToDouble(QString coords);
    bool isValidCoordsFormat(QString coordString);

//modify for binary search.....................
    bool isValidNavaid(QString name);
    bool isFix(QString name);
    bool isVOR(QString name);
    bool isNDB(QString name);
    bool isAirport(QString name);
//..............................................

    void appendSector(ATCAirspaceSector *sector);
    void appendFix(ATCNavFix *fix);
    void appendVOR(ATCBeaconVOR *vor);
    void appendNDB(ATCBeaconNDB *ndb);
    void appendAirport(ATCAirport *airport);
    void appendSID(ATCProcedureSID *sid);
    void appendSTAR(ATCProcedureSTAR *star);
    void appendSIDSymbol(ATCProcedureSIDSymbol *symbol);
    void appendSTARSymbol(ATCProcedureSTARSymbol *symbol);

    int getSectorVectorSize();
    int getFixesVectorSize();
    int getVORsVectorSize();
    int getNDBsVectorSize();
    int getAirportsVectorSize();
    int getSIDsVectorSize();
    int getSTARsVectorSize();
    int getSIDSymbolsVectorSize();
    int getSTARSymbolsVectorSize();

    ATCAirspaceSector* getSector(int iterator);
    ATCAirspaceSector* getLastSector();
    ATCNavFix* getFix(int iterator);
    ATCBeaconVOR* getVOR(int iterator);
    ATCBeaconNDB* getNDB(int iterator);
    ATCAirport* getAirport(int iterator);
    ATCProcedureSID* getSID(int iterator);
    ATCProcedureSTAR* getSTAR(int iterator);
    ATCProcedureSIDSymbol* getSIDSymbol(int iterator);
    ATCProcedureSTARSymbol* getSTARSymbol(int iterator);

    double getNavaidLatitude(QString name);
    double getNavaidLongitude(QString name);

    ATCAirport* findAirport(QString ICAOname);
    ATCNavFix* findFix(QString fixName);
    ATCBeaconVOR* findVOR(QString name);
    ATCBeaconNDB* findNDB(QString name);

private:
    QVector<ATCAirspaceSector*> sectors;
    QVector<ATCNavFix*> fixes;
    QVector<ATCBeaconVOR*> vors;
    QVector<ATCBeaconNDB*> ndbs;
    QVector<ATCAirport*> airports;
    QVector<ATCProcedureSID*> sids;
    QVector<ATCProcedureSTAR*> stars;
    QVector<ATCProcedureSIDSymbol*> sidSymbols;
    QVector<ATCProcedureSTARSymbol*> starSymbols;

    void deleteAllSectors();
    void deleteAllFixes();
    void deleteAllVORs();
    void deleteAllNDBs();
    void deleteAllAirports();
    void deleteAllSIDs();
    void deleteAllSTARs();
    void deleteAllSIDSymbols();
    void deleteAllSTARSymbols();
};

#endif // ATCAIRSPACE_H
