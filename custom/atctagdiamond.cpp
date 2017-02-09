
#include "atctagdiamond.h"

ATCTagDiamond::ATCTagDiamond(double x, double y, double width, double height, ATCSettings *settings, double *scale) :
    QGraphicsRectItem(x, y, width, height),
    settings(settings),
    scale(scale)
{
    tempPos = QPointF(0, 0);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

ATCTagDiamond::~ATCTagDiamond()
{

}

void ATCTagDiamond::setConnector(QGraphicsLineItem *connector)
{
    line = connector;
}

void ATCTagDiamond::moveLine(QPointF newPos)
{
    int centreX = rect().x() + settings->TAG_DIAMOND_WIDTH / *scale / 2;
    int centreY = rect().y() + settings->TAG_DIAMOND_WIDTH / *scale / 2;

    QPointF newCentrePos = QPointF(newPos.x() + centreX, newPos.y() + centreY);

    QPointF p1 = newCentrePos;
    QPointF p2 = line->line().p2();

    line->setLine(QLineF(p1, p2));
}

QVariant ATCTagDiamond::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionChange && scene())
    {
        tempPos = value.toPointF();
        moveLine(tempPos);
    }

    return QGraphicsItem::itemChange(change, value);
}
