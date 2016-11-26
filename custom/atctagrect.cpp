
#include "atctagrect.h"


ATCTagRect::ATCTagRect(double x, double y, double width, double height, ATCSettings *settings, double *scale, ATC::TagType *type) :
    QGraphicsRectItem(x, y, width, height),
    settings(settings),
    scale(scale),
    type(type)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setAcceptHoverEvents(true);
}

ATCTagRect::~ATCTagRect()
{

}

QString ATCTagRect::getShortEtiquette()
{
    return shortEtiquette;
}

QString ATCTagRect::getLongEtiquette()
{
    return longEtiquette;
}

void ATCTagRect::setShortEtiquette(QString &string)
{
    shortEtiquette = string;
}

void ATCTagRect::setLongEtiquette(QString &string)
{
    longEtiquette = string;
}

void ATCTagRect::setShort()
{
    text->setText(shortEtiquette);
}

void ATCTagRect::setLong()
{
    text->setText(longEtiquette);
}

void ATCTagRect::setConnector(QGraphicsLineItem *connector)
{
    line = connector;
}

void ATCTagRect::setText(QGraphicsSimpleTextItem *txt)
{
    text = txt;
}

void ATCTagRect::moveLine(QPointF newPos)
{
    int centreX = rect().x() + settings->TAG_BOX_WIDTH / *scale / 2;
    int centreY;

    if(*type == ATC::Short)
    {
        centreY = rect().y() + settings->TAG_BOX_HEIGHT / *scale / 2;
    }
    else
    {
        centreY = rect().y() + settings->TAG_BOX_HEIGHT_FULL / *scale / 2;
    }

    QPointF newCentrePos = QPointF(newPos.x() + centreX, newPos.y() + centreY);

    QPointF p1 = line->line().p1();
    QPointF p2 = newCentrePos;

    double alpha = atan2(p2.y() - p1.y(), p2.x() - p1.x());

    if((alpha > - ATCConst::PI/4) && (alpha <= ATCConst::PI/4))
    {
        p2 = QPointF(newCentrePos.x() - settings->TAG_BOX_WIDTH / *scale / 2, newCentrePos.y());
    }
    else if((alpha > ATCConst::PI/4) && (alpha <= 3*ATCConst::PI/4))
    {
        p2 = QPointF(newCentrePos.x(), newCentrePos.y() - settings->TAG_BOX_HEIGHT / *scale / 2);
    }
    else if((alpha > - 3*ATCConst::PI/4) && (alpha <=  - ATCConst::PI/4))
    {
        if(*type == ATC::Short)
        {
            p2 = QPointF(newCentrePos.x(), newCentrePos.y() + settings->TAG_BOX_HEIGHT / *scale / 2);
        }
        else
        {
            p2 = QPointF(newCentrePos.x(), newCentrePos.y() + settings->TAG_BOX_HEIGHT_FULL / *scale / 2);
        }
    }
    else
    {
        p2 = QPointF(newCentrePos.x() + settings->TAG_BOX_WIDTH / *scale / 2, newCentrePos.y());
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

void ATCTagRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if(*type == ATC::Short)
    {
        setRect(rect().x(), rect().y(), settings->TAG_BOX_WIDTH / *scale ,settings->TAG_BOX_HEIGHT_FULL / *scale);
        setOpacity(1);

        setLong();
    }
    else
    {
        setOpacity(1);
    }
}

void ATCTagRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if(*type == ATC::Short)
    {
        setRect(rect().x(), rect().y(), settings->TAG_BOX_WIDTH / *scale ,settings->TAG_BOX_HEIGHT / *scale);
        setOpacity(0.01);

        setShort();
    }
    else
    {
        setOpacity(0.01);
    }
}
