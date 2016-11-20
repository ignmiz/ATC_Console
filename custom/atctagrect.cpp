
#include "atctagrect.h"


ATCTagRect::ATCTagRect(double x, double y, double width, double height, QGraphicsLineItem *connector) :
    QGraphicsRectItem(x, y, width, height),
    line(connector)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

ATCTagRect::~ATCTagRect()
{

}

void ATCTagRect::moveLine(QPointF newPos)
{
    int centreX = rect().x() + rect().width()/2;
    int centreY = rect().y() + rect().height()/2;

    QPointF newCentrePos = QPointF(newPos.x() + centreX, newPos.y() + centreY);

    QPointF p1 = line->line().p1();
    QPointF p2 = newCentrePos;

    line->setLine(QLineF(p1, p2));
}

QVariant ATCTagRect::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionChange && scene())
    {
        QPointF newPos = value.toPointF();
        moveLine(newPos);
    }

    return QGraphicsItem::itemChange(change, value);
}
