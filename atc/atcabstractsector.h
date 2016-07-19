#ifndef ATCABSTRACTSECTOR_H
#define ATCABSTRACTSECTOR_H

#include "atcairspacefix.h"
#include <QGraphicsItem>

struct coordsPair{
    double x1;
    double y1;
    double x2;
    double y2;
};

class ATCAbstractSector
{
public:
    explicit ATCAbstractSector(QString name);
    virtual ~ATCAbstractSector() = 0;

    QString getName();
    ATCAirspaceFix* getCoords1(int iterator);
    ATCAirspaceFix* getCoords2(int iterator);
    coordsPair& getCoordsPair(int iterator);
    QGraphicsLineItem* getLine(int iterator);
    int getCoordsVectorSize();

    void setName(QString name);

    void appendCoords1(ATCAirspaceFix* coord);
    void appendCoords2(ATCAirspaceFix *coord);
    void appendCoordsPair(coordsPair &pair);
    void appendLine(QGraphicsLineItem *line);

private:
    QString sectorName;
    QVector<ATCAirspaceFix*> coords1;
    QVector<ATCAirspaceFix*> coords2;
    QVector<QGraphicsLineItem*> lines;
    QVector<coordsPair> projectedCoords;
};

#endif // ATCABSTRACTSECTOR_H
