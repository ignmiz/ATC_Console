#ifndef ATCAMANFLIGHTLABEL_H
#define ATCAMANFLIGHTLABEL_H

#include <QGraphicsItem>

class ATCAmanFlightLabel
{
public:
    explicit ATCAmanFlightLabel(QPointF arrowPos);
    ~ATCAmanFlightLabel();

private:
    QGraphicsLineItem *timeArrow = nullptr;
    QGraphicsLineItem *connector = nullptr;
    QGraphicsPolygonItem *polygon = nullptr;
    QGraphicsSimpleTextItem *text = nullptr;
};

#endif // ATCAMANFLIGHTLABEL_H
