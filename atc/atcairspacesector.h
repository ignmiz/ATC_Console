#ifndef ATCAIRSPACESECTOR_H
#define ATCAIRSPACESECTOR_H

#include "atcairspacefix.h"

#include <QString>
#include <QVector>

class ATCAirspaceSector
{

public:
    explicit ATCAirspaceSector();
    explicit ATCAirspaceSector(QString name);
    ~ATCAirspaceSector();

    QString getSectorName();
    ATCAirspaceFix* getCoordinates(int iterator);
    int getCoordinatesVectorSize();

    void setSectorName(QString name);

    void appendAirspaceFix(ATCAirspaceFix *airspaceFix);
    void deleteAllAirspaceFixes();

private:
    QString sectorName;
    QVector<ATCAirspaceFix*> coordinates;
};

#endif // ATCAIRSPACESECTOR_H
