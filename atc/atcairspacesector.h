#ifndef ATCAIRSPACESECTOR_H
#define ATCAIRSPACESECTOR_H

#include "atcairspacefix.h"

#include <QString>
#include <QVector>
#include <QGraphicsItem>

class ATCAirspaceSector
{

public:
    explicit ATCAirspaceSector();
    explicit ATCAirspaceSector(QString name);
    ~ATCAirspaceSector();

    QString getSectorName();
    ATCAirspaceFix* getCoordinates(int iterator);
    int getCoordinatesVectorSize();
    QGraphicsPolygonItem* getPolygon();

    void setSectorName(QString name);
    void setPolygon(QGraphicsPolygonItem *sectorPolygon);

    void appendAirspaceFix(ATCAirspaceFix *airspaceFix);
    void deleteAllAirspaceFixes();

private:
    QString sectorName;
    QVector<ATCAirspaceFix*> coordinates;
    QGraphicsPolygonItem *polygon;

};

#endif // ATCAIRSPACESECTOR_H
