#ifndef ATCTAGRECT_H
#define ATCTAGRECT_H

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
};

#endif // ATCTAGRECT_H
