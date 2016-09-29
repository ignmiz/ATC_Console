#include "atcabstractbeacon.h"

#include <QDebug>

ATCAbstractBeacon::ATCAbstractBeacon(QString beaconName, float beaconFrequency, double latitude, double longitude) :
    name(beaconName),
    frequency(beaconFrequency),
    QGeoCoordinate(latitude, longitude)
{
}

ATCAbstractBeacon::~ATCAbstractBeacon()
{
    if(scenePosition != nullptr) delete scenePosition;
    if(symbol != nullptr) delete symbol;
    if(label != nullptr) delete label;
}

QString ATCAbstractBeacon::getName() const
{
    return name;
}

float ATCAbstractBeacon::getFrequency() const
{
    return frequency;
}

QAbstractGraphicsShapeItem *ATCAbstractBeacon::getSymbol() const
{
    return symbol;
}

QPointF* ATCAbstractBeacon::getScenePosition() const
{
    return scenePosition;
}

QGraphicsSimpleTextItem* ATCAbstractBeacon::getLabel() const
{
    return label;
}

void ATCAbstractBeacon::setName(QString beaconName)
{
    name = beaconName;
}

void ATCAbstractBeacon::setFrequency(float beaconFrequency)
{
    frequency = beaconFrequency;
}

void ATCAbstractBeacon::setSymbol(QAbstractGraphicsShapeItem *graphicsItem)
{
    symbol = graphicsItem;
}

void ATCAbstractBeacon::setScenePosition(QPointF *position)
{
    scenePosition = position;
}

void ATCAbstractBeacon::setLabel(QGraphicsSimpleTextItem *textLabel)
{
    label = textLabel;
}

void ATCAbstractBeacon::setColor(QColor color)
{
    QPen newPen(symbol->pen());
    newPen.setColor(color);

    symbol->setPen(newPen);
}

void ATCAbstractBeacon::show()
{
    symbol->show();
    label->show();

    visible = true;
}

void ATCAbstractBeacon::hide()
{
    symbol->hide();
    label->hide();

    visible = false;
}

bool ATCAbstractBeacon::isVisible()
{
    return visible;
}

void ATCAbstractBeacon::setFlagVisible(bool flag)
{
    visible = flag;
}
