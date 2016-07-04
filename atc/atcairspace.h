#ifndef ATCAIRSPACE_H
#define ATCAIRSPACE_H

#include "atcairspacesector.h"
#include "atcnavfix.h"
#include "atcbeaconvor.h"
#include "atcbeaconndb.h"
#include "atcairport.h"

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

    void deleteAllSectors();
    void deleteAllFixes();
    void deleteAllVORs();
    void deleteAllNDBs();
    void deleteAllAirports();

    ATCAirspaceSector* getSector(int iterator);
    ATCAirspaceSector* getLastSector();
    int getSectorVectorSize();
    int getFixesVectorSize();
    int getVORsVectorSize();
    int getNDBsVectorSize();
    int getAirportsVectorSize();
    ATCNavFix* getFix(int iterator);
    ATCBeaconVOR* getVOR(int iterator);
    ATCBeaconNDB* getNDB(int iterator);
    ATCAirport* getAirport(int iterator);

    ATCAirport* findAirport(QString ICAOname);

private:
    QVector<ATCAirspaceSector*> sectors;
    QVector<ATCNavFix*> fixes;
    QVector<ATCBeaconVOR*> vors;
    QVector<ATCBeaconNDB*> ndbs;
    QVector<ATCAirport*> airports;
};

#endif // ATCAIRSPACE_H
