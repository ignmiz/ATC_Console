#ifndef ATCAIRSPACE_H
#define ATCAIRSPACE_H

#include "atcairspacesector.h"
#include "atcnavfix.h"

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

    void deleteAllSectors();
    void deleteAllFixes();

    ATCAirspaceSector* getSector(int iterator);
    ATCAirspaceSector* getLastSector();
    int getSectorVectorSize();
    ATCNavFix* getFix(int iterator);

private:
    QVector<ATCAirspaceSector*> sectors;
    QVector<ATCNavFix*> fixes;
};

#endif // ATCAIRSPACE_H
