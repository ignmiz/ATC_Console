#ifndef ATCAMANFLIGHTLABEL_H
#define ATCAMANFLIGHTLABEL_H

#include "atcflight.h"

#include <QPen>
#include <QPolygonF>
#include <QGraphicsItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsScene>

enum class SelectStatus
{
    Inactive, Active
};

class ATCAmanFlightLabel : public QGraphicsPolygonItem
{
public:
    explicit ATCAmanFlightLabel(ATCFlight *flight, QPointF arrowPos);
    ~ATCAmanFlightLabel();

    void addToScene(QGraphicsScene *scene);
    void moveByInterval(double dx, double dy);

private:
    ATCFlight *flight;
    QPointF arrowPos;

    QGraphicsLineItem *timeArrow = nullptr;
    QGraphicsLineItem *connector = nullptr;
    QGraphicsSimpleTextItem *text = nullptr;

    void createLabelItems(QPointF arrowPos);

    void moveLine(QPointF newPos);

    void swapLine(QGraphicsLineItem *line);
    void swapPolygon(QGraphicsPolygonItem *polygon);
    void swapText(QGraphicsSimpleTextItem *text);
    void swapSide();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // ATCAMANFLIGHTLABEL_H
