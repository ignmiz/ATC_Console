
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

    double alpha = atan2(p2.y() - p1.y(), p2.x() - p1.x());

    if((alpha > - ATCConst::PI/4) && (alpha <= ATCConst::PI/4))
    {
        p2 = QPointF(newCentrePos.x() - rect().width()/2, newCentrePos.y());
    }
    else if((alpha > ATCConst::PI/4) && (alpha <= 3*ATCConst::PI/4))
    {
        p2 = QPointF(newCentrePos.x(), newCentrePos.y() - rect().height()/2);
    }
    else if((alpha > - 3*ATCConst::PI/4) && (alpha <=  - ATCConst::PI/4))
    {
        p2 = QPointF(newCentrePos.x(), newCentrePos.y() + rect().height()/2);
    }
    else
    {
        p2 = QPointF(newCentrePos.x() + rect().width()/2, newCentrePos.y());
    }

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
