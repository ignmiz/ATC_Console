#ifndef ATCTAGDIAMOND_H
#define ATCTAGDIAMOND_H

#include "atcsettings.h"

#include <QGraphicsRectItem>

class ATCTagDiamond : public QGraphicsRectItem
{
public:
    explicit ATCTagDiamond(double x, double y, double width, double height, ATCSettings *settings, double *scale);
    ~ATCTagDiamond();

    void setConnector(QGraphicsLineItem *connector);

private:
    void moveLine(QPointF newPos);

    QGraphicsLineItem *line;
    ATCSettings *settings;
    double *scale;
    QPointF tempPos;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // ATCTAGDIAMOND_H
