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
#include "atcairwaylow.h"
#include "atcairwayhigh.h"

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
    bool isAirwayLow(QString name);
    bool isAirwayHigh(QString name);
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
    void appendAirwayLow(ATCAirwayLow *airway);
    void appendAirwayHigh(ATCAirwayHigh *airway);

    int getSectorVectorSize();
    int getFixesVectorSize();
    int getVORsVectorSize();
    int getNDBsVectorSize();
    int getAirportsVectorSize();
    int getSIDsVectorSize();
    int getSTARsVectorSize();
    int getSIDSymbolsVectorSize();
    int getSTARSymbolsVectorSize();
    int getAirwayLowVectorSize();
    int getAirwayHighVectorSize();

    ATCAirspaceSector* getSector(int iterator);
    ATCNavFix* getFix(int iterator);
    ATCBeaconVOR* getVOR(int iterator);
    ATCBeaconNDB* getNDB(int iterator);
    ATCAirport* getAirport(int iterator);
    ATCProcedureSID* getSID(int iterator);
    ATCProcedureSTAR* getSTAR(int iterator);
    ATCProcedureSIDSymbol* getSIDSymbol(int iterator);
    ATCProcedureSTARSymbol* getSTARSymbol(int iterator);
    ATCAirwayLow* getAirwayLow(int iterator);
    ATCAirwayHigh* getAirwayHigh(int iterator);

    ATCAirspaceSector* getLastSector();
    ATCProcedureSIDSymbol* getLastSIDSymbol();
    ATCProcedureSTARSymbol* getLastSTARSymbol();
    ATCAirwayLow* getLastAirwayLow();
    ATCAirwayHigh* getLastAirwayHigh();

    double getNavaidLatitude(QString name);
    double getNavaidLongitude(QString name);

    ATCAirport* findAirport(QString ICAOname);
    ATCNavFix* findFix(QString fixName);
    ATCBeaconVOR* findVOR(QString name);
    ATCBeaconNDB* findNDB(QString name);
    ATCAirwayLow* findAirwayLow(QString name);
    ATCAirwayHigh* findAirwayHigh(QString name);

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
    QVector<ATCAirwayLow*> lowAirways;
    QVector<ATCAirwayHigh*> highAirways;

    void deleteAllSectors();
    void deleteAllFixes();
    void deleteAllVORs();
    void deleteAllNDBs();
    void deleteAllAirports();
    void deleteAllSIDs();
    void deleteAllSTARs();
    void deleteAllSIDSymbols();
    void deleteAllSTARSymbols();
    void deleteAllAirwayLow();
    void deleteAllAirwayHigh();
};

#endif // ATCAIRSPACE_H
