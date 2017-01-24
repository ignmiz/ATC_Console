#ifndef ATCAIRSPACE_H
#define ATCAIRSPACE_H

#include "atcairspacesector.h"
#include "atcsectorartcclow.h"
#include "atcsectorartcchigh.h"
#include "atcsectorartcc.h"
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

#include <QFile>
#include <QString>
#include <QVector>

class ATCAirspace
{

public:
    explicit ATCAirspace(QString SCTpath, QString ESEpath);
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
    void appendSectorARTCCLow(ATCSectorARTCCLow *sector);
    void appendSectorARTCCHigh(ATCSectorARTCCHigh *sector);
    void appendSectorARTCC(ATCSectorARTCC *sector);
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
    int getSectorARTCCLowVectorSize();
    int getSectorARTCCHighVectorSize();
    int getSectorARTCCVectorSize();
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
    ATCSectorARTCCLow* getSectorARTCCLow(int iterator);
    ATCSectorARTCCHigh* getSectorARTCCHigh(int iterator);
    ATCSectorARTCC* getSectorARTCC(int iterator);
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

    QVector<ATCSectorARTCCLow*>& getSectorARTCCLowVector();
    QVector<ATCSectorARTCCHigh*>& getSectorARTCCHighVector();
    QVector<ATCSectorARTCC*>& getSectorARTCCVector();
    QVector<ATCNavFix*>& getFixesVector();
    QVector<ATCBeaconNDB*>& getNDBsVector();
    QVector<ATCBeaconVOR*>& getVORsVector();
    QVector<ATCAirport*>& getAirportsVector();
    QVector<ATCProcedureSIDSymbol*>& getSIDSymbolsVector();
    QVector<ATCProcedureSTARSymbol*>& getSTARSymbolsVector();
    QVector<ATCAirwayLow*>& getAirwayLowVector();
    QVector<ATCAirwayHigh*>& getAirwayHighVector();

    ATCAirspaceSector* getLastSector();
    ATCSectorARTCCLow* getLastSectorARTCCLow();
    ATCSectorARTCCHigh* getLastSectorARTCCHigh();
    ATCSectorARTCC* getLastSectorARTCC();
    ATCProcedureSIDSymbol* getLastSIDSymbol();
    ATCProcedureSTARSymbol* getLastSTARSymbol();
    ATCAirwayLow* getLastAirwayLow();
    ATCAirwayHigh* getLastAirwayHigh();

    double getNavaidLatitude(QString name);
    double getNavaidLongitude(QString name);

    ATCSectorARTCCLow* findSectorARTCCLow(QString name);
    ATCSectorARTCCHigh* findSectorARTCCHigh(QString name);
    ATCSectorARTCC* findSectorARTCC(QString name);
    ATCAirport* findAirport(QString ICAOname);
    ATCRunwayExtendedCentreline* findCentreline(QString airportName, QString rwyID);
    ATCNavFix* findFix(QString fixName);
    ATCBeaconVOR* findVOR(QString name);
    ATCBeaconNDB* findNDB(QString name);
    ATCProcedureSID* findSID(QString name);
    ATCProcedureSTAR* findSTAR(QString name);
    ATCProcedureSIDSymbol* findSIDSymbol(QString name);
    ATCProcedureSTARSymbol* findSTARSymbol(QString name);
    ATCAirwayLow* findAirwayLow(QString name);
    ATCAirwayHigh* findAirwayHigh(QString name);
    ATCAbstractAirway* findAirway(QString name);

private:
    QVector<ATCAirspaceSector*> sectors;
    QVector<ATCSectorARTCCLow*> sectorsARTCCLow;
    QVector<ATCSectorARTCCHigh*> sectorsARTCCHigh;
    QVector<ATCSectorARTCC*> sectorsARTCC;
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
    void deleteAllSectorsARTCCLow();
    void deleteAllSectorsARTCCHigh();
    void deleteAllSectorsARTCC();
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

    void loadData(QString SCTpath, QString ESEpath);
};

#endif // ATCAIRSPACE_H
