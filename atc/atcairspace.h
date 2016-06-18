#ifndef ATCAIRSPACE_H
#define ATCAIRSPACE_H

#include "atcairspacesector.h"

#include <QString>
#include <QVector>

class ATCAirspace
{

public:
    explicit ATCAirspace();
//    explicit ATCAirspace(QString name);
    ~ATCAirspace();

//    QString getAirspaceName();
//    void setAirspaceName(QString name);
    double coordsStringToDouble(QString coords);
    void appendSector(ATCAirspaceSector *sector);
    void deleteAllSectors();

    ATCAirspaceSector* getSector(int iterator);
    ATCAirspaceSector* getLastSector();

private:
//    QString airspaceName;
    QVector<ATCAirspaceSector*> sectors;
};

#endif // ATCAIRSPACE_H
