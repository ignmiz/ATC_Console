#ifndef ATCABSTRACTSECTOR_H
#define ATCABSTRACTSECTOR_H

#include "atcairspacefix.h"
#include <QGraphicsItem>
#include <QPen>

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
    QGraphicsPolygonItem* getPolygon(int iterator);

    int getCoordsVectorSize();
    int getPolygonsVectorSize();

    void setName(QString name);
    void setColor(QColor color);

    void appendCoords1(ATCAirspaceFix* coord);
    void appendCoords2(ATCAirspaceFix *coord);
    void appendCoordsPair(coordsPair &pair);
    void appendPolygon(QGraphicsPolygonItem *polygon);

    void show();
    void hide();
    bool isVisible();

private:
    QString sectorName;
    QVector<ATCAirspaceFix*> coords1;
    QVector<ATCAirspaceFix*> coords2;
    QVector<QGraphicsLineItem*> lines;
    QVector<coordsPair> projectedCoords;
    QVector<QGraphicsPolygonItem*> polygons;

    bool visible = false;
};

#endif // ATCABSTRACTSECTOR_H
