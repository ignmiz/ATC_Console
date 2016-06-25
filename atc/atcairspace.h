#ifndef ATCAIRSPACE_H
#define ATCAIRSPACE_H

#include "atcairspacesector.h"
#include "atcnavfix.h"
#include "atcbeaconvor.h"
#include "atcbeaconndb.h"

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

    void deleteAllSectors();
    void deleteAllFixes();
    void deleteAllVORs();
    void deleteAllNDBs();

    ATCAirspaceSector* getSector(int iterator);
    ATCAirspaceSector* getLastSector();
    int getSectorVectorSize();
    ATCNavFix* getFix(int iterator);
    ATCBeaconVOR* getVOR(int iterator);
    ATCBeaconNDB* getNDB(int iterator);

private:
    QVector<ATCAirspaceSector*> sectors;
    QVector<ATCNavFix*> fixes;
    QVector<ATCBeaconVOR*> vors;
    QVector<ATCBeaconNDB*> ndbs;
};

#endif // ATCAIRSPACE_H
