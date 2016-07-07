#ifndef ATCAIRSPACE_H
#define ATCAIRSPACE_H

#include "atcairspacesector.h"
#include "atcnavfix.h"
#include "atcbeaconvor.h"
#include "atcbeaconndb.h"
#include "atcairport.h"
#include "atcproceduresid.h"
#include "atcprocedurestar.h"

#include <QString>
#include <QVector>

class ATCAirspace
{

public:
    explicit ATCAirspace();
    ~ATCAirspace();

    double coordsStringToDouble(QString coords);

    void appendSector(ATCAirspaceSector *sector);
    void appendFix(ATCNavFix *fix);
    void appendVOR(ATCBeaconVOR *vor);
    void appendNDB(ATCBeaconNDB *ndb);
    void appendAirport(ATCAirport *airport);
    void appendSID(ATCProcedureSID *sid);
    void appendSTAR(ATCProcedureSTAR *star);

    ATCAirspaceSector* getSector(int iterator);
    ATCAirspaceSector* getLastSector();
    int getSectorVectorSize();
    int getFixesVectorSize();
    int getVORsVectorSize();
    int getNDBsVectorSize();
    int getAirportsVectorSize();
    int getSIDsVectorSize();
    int getSTARsVectorSize();
    ATCNavFix* getFix(int iterator);
    ATCBeaconVOR* getVOR(int iterator);
    ATCBeaconNDB* getNDB(int iterator);
    ATCAirport* getAirport(int iterator);
    ATCProcedureSID* getSID(int iterator);
    ATCProcedureSTAR* getSTAR(int iterator);

    ATCAirport* findAirport(QString ICAOname);
    ATCNavFix* findFix(QString fixName);

private:
    QVector<ATCAirspaceSector*> sectors;
    QVector<ATCNavFix*> fixes;
    QVector<ATCBeaconVOR*> vors;
    QVector<ATCBeaconNDB*> ndbs;
    QVector<ATCAirport*> airports;
    QVector<ATCProcedureSID*> sids;
    QVector<ATCProcedureSTAR*> stars;

    void deleteAllSectors();
    void deleteAllFixes();
    void deleteAllVORs();
    void deleteAllNDBs();
    void deleteAllAirports();
    void deleteAllSIDs();
    void deleteAllSTARs();
};

#endif // ATCAIRSPACE_H
