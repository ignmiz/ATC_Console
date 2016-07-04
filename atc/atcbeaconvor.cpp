#include "atcbeaconvor.h"
#include <QDebug>

ATCBeaconVOR::ATCBeaconVOR(QString beaconName, float beaconFrequency, double latitude, double longitude) :
    name(beaconName),
    frequency(beaconFrequency),
    QGeoCoordinate(latitude, longitude)
{
}

ATCBeaconVOR::~ATCBeaconVOR()
{
    if(scenePosition != nullptr) delete scenePosition;
    if(symbol != nullptr) delete symbol;
    if(label != nullptr) delete label;

    qDebug() << "VOR: " << name << " : " << frequency << " : " << toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere) << " deleted...";
}

QString ATCBeaconVOR::getName()
{
    return name;
}

float ATCBeaconVOR::getFrequency()
{
    return frequency;
}

QGraphicsRectItem* ATCBeaconVOR::getSymbol()
{
    return symbol;
}

QPointF* ATCBeaconVOR::getScenePosition()
{
    return scenePosition;
}

QGraphicsSimpleTextItem* ATCBeaconVOR::getLabel()
{
    return label;
}

void ATCBeaconVOR::setName(QString beaconName)
{
    name = beaconName;
}

void ATCBeaconVOR::setFrequency(float beaconFrequency)
{
    frequency = beaconFrequency;
}

void ATCBeaconVOR::setSymbol(QGraphicsRectItem *vorSymbol)
{
    symbol = vorSymbol;
}

void ATCBeaconVOR::setScenePosition(QPointF *position)
{
    scenePosition = position;
}

void ATCBeaconVOR::setLabel(QGraphicsSimpleTextItem *textLabel)
{
    label = textLabel;
}
