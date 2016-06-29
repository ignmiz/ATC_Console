#include "atcnavfix.h"
#include <QDebug>

ATCNavFix::ATCNavFix(QString fixName, double latitude, double longitude) : QGeoCoordinate(latitude, longitude), name(fixName)
{
}

ATCNavFix::~ATCNavFix()
{
    delete scenePosition;
    delete symbol;
    delete label;
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

QPointF* ATCNavFix::getScenePosiiton()
{
    return scenePosition;
}

QGraphicsSimpleTextItem *ATCNavFix::getLabel()
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
