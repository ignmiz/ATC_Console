#include "atcbeaconndb.h"
#include <QDebug>

ATCBeaconNDB::ATCBeaconNDB(QString beaconName, float beaconFrequency, double latitude, double longitude) :
    name(beaconName),
    frequency(beaconFrequency),
    QGeoCoordinate(latitude, longitude)
{
}

ATCBeaconNDB::~ATCBeaconNDB()
{
    if(scenePosition != nullptr) delete scenePosition;
    if(symbol != nullptr) delete symbol;
    if(label != nullptr) delete label;

    qDebug() << "NDB: " << name << " : " << frequency << " : " << toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere) << " deleted...";
}

QString ATCBeaconNDB::getName()
{
    return name;
}

float ATCBeaconNDB::getFrequency()
{
    return frequency;
}

QGraphicsEllipseItem* ATCBeaconNDB::getSymbol()
{
    return symbol;
}

QPointF* ATCBeaconNDB::getScenePosition()
{
    return scenePosition;
}

QGraphicsSimpleTextItem* ATCBeaconNDB::getLabel()
{
    return label;
}

void ATCBeaconNDB::setName(QString beaconName)
{
    name = beaconName;
}

void ATCBeaconNDB::setFrequency(float beaconFrequency)
{
    frequency = beaconFrequency;
}

void ATCBeaconNDB::setSymbol(QGraphicsEllipseItem *ndbSymbol)
{
    symbol = ndbSymbol;
}

void ATCBeaconNDB::setScenePosition(QPointF *position)
{
    scenePosition = position;
}

void ATCBeaconNDB::setLabel(QGraphicsSimpleTextItem *textLabel)
{
    label = textLabel;
}
