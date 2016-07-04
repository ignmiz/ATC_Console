#include "atcnavfix.h"
#include <QDebug>

ATCNavFix::ATCNavFix(QString fixName, double latitude, double longitude) : QGeoCoordinate(latitude, longitude), name(fixName)
{
}

ATCNavFix::~ATCNavFix()
{
    if(scenePosition != nullptr) delete scenePosition;
    if(symbol != nullptr) delete symbol;
    if(label != nullptr) delete label;
    qDebug() << name << ": " << toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere) << " deleted...";
}

QString ATCNavFix::getName()
{
    return name;
}

QGraphicsPolygonItem* ATCNavFix::getSymbol()
{
    return symbol;
}

QPointF* ATCNavFix::getScenePosition()
{
    return scenePosition;
}

QGraphicsSimpleTextItem* ATCNavFix::getLabel()
{
    return label;
}

void ATCNavFix::setName(QString fixName)
{
    name = fixName;
}

void ATCNavFix::setSymbol(QGraphicsPolygonItem *fixSymbol)
{
    symbol = fixSymbol;
}

void ATCNavFix::setScenePosition(QPointF *position)
{
    scenePosition = position;
}

void ATCNavFix::setLabel(QGraphicsSimpleTextItem *textLabel)
{
    label = textLabel;
}
