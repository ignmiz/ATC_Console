#ifndef ATCBEACONNDB_H
#define ATCBEACONNDB_H

#include <QGeoCoordinate>
#include <QGraphicsItem>

class ATCBeaconNDB : public QGeoCoordinate
{

public:
    explicit ATCBeaconNDB(QString beaconName, float beaconFrequency, double latitude, double longitude);
    ~ATCBeaconNDB();

    QString getName();
    float getFrequency();
    QGraphicsEllipseItem* getSymbol();
    QPointF* getScenePosition();
    QGraphicsSimpleTextItem* getLabel();

    void setName(QString beaconName);
    void setFrequency(float beaconFrequency);
    void setSymbol(QGraphicsEllipseItem *ndbSymbol);
    void setScenePosition(QPointF *position);
    void setLabel(QGraphicsSimpleTextItem *textLabel);

private:
    QString name;
    float frequency;
    QPointF *scenePosition = nullptr;
    QGraphicsEllipseItem *symbol = nullptr;
    QGraphicsSimpleTextItem *label = nullptr;

};

#endif // ATCBEACONNDB_H
