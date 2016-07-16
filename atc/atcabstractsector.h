#ifndef ATCABSTRACTSECTOR_H
#define ATCABSTRACTSECTOR_H

#include "atcairspacefix.h"
#include <QGraphicsItem>

class ATCAbstractSector
{
public:
    explicit ATCAbstractSector(QString name);
    virtual ~ATCAbstractSector() = 0;

    QString getName();
    ATCAirspaceFix* getCoords1(int iterator);
    ATCAirspaceFix* getCoords2(int iterator);
    QGraphicsLineItem* getLine(int iterator);
    int getCoordsVectorSize();

    void setName(QString name);

    void appendCoords1(ATCAirspaceFix* coord);
    void appendCoords2(ATCAirspaceFix *coord);
    void appendLine(QGraphicsLineItem *line);

private:
    QString sectorName;
    QVector<ATCAirspaceFix*> coords1;
    QVector<ATCAirspaceFix*> coords2;
    QVector<QGraphicsLineItem*> lines;
};

#endif // ATCABSTRACTSECTOR_H
