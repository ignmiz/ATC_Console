#ifndef ATCABSTRACTAIRWAY_H
#define ATCABSTRACTAIRWAY_H

#include "atcairspacefix.h"

#include <QGraphicsItem>

class ATCAbstractAirway
{
public:
    explicit ATCAbstractAirway(QString name);
    virtual ~ATCAbstractAirway() = 0;

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
    QString airwayName;
    QVector<ATCAirspaceFix*> coords1;
    QVector<ATCAirspaceFix*> coords2;
    QVector<QGraphicsLineItem*> lines;
};

#endif // ATCABSTRACTAIRWAY_H
