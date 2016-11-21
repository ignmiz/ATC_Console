#ifndef ATCTAGRECT_H
#define ATCTAGRECT_H

#include "atcconst.h"

#include <QGraphicsItem>

class ATCTagRect : public QGraphicsRectItem
{
public:
    explicit ATCTagRect(double x, double y, double width, double height, QGraphicsLineItem *connector);
    ~ATCTagRect();

private:
    void moveLine(QPointF newPos);

    QGraphicsLineItem *line;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // ATCTAGRECT_H
