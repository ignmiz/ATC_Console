#ifndef ATCABSTRACTAIRWAY_H
#define ATCABSTRACTAIRWAY_H

#include "atcairspacefix.h"

#include <QGraphicsItem>
#include <QPen>

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
    QStringList& getRouteFixes();

    void setName(QString name);
    void setColor(QColor color);

    void appendCoords1(ATCAirspaceFix* coord);
    void appendCoords2(ATCAirspaceFix *coord);
    void appendLine(QGraphicsLineItem *line);
    void appendRouteFix(QString fix);

    void show();
    void hide();
    bool isVisible();

private:
    QString airwayName;
    QVector<ATCAirspaceFix*> coords1;
    QVector<ATCAirspaceFix*> coords2;
    QVector<QGraphicsLineItem*> lines;
    QStringList routeFixes;

    bool visible = false;
};

#endif // ATCABSTRACTAIRWAY_H
