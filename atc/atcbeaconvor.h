#ifndef ATCBEACONVOR_H
#define ATCBEACONVOR_H

#include <QGeoCoordinate>
#include <QGraphicsItem>

class ATCBeaconVOR : public QGeoCoordinate
{

public:
    explicit ATCBeaconVOR(QString beaconName, float beaconFrequency, double latitude, double longitude);
    ~ATCBeaconVOR();

    QString getName();
    float getFrequency();
    QGraphicsRectItem* getSymbol();
    QPointF* getScenePosition();
    QGraphicsSimpleTextItem* getLabel();

    void setName(QString beaconName);
    void setFrequency(float beaconFrequency);
    void setSymbol(QGraphicsRectItem *vorSymbol);
    void setScenePosition(QPointF *position);
    void setLabel(QGraphicsSimpleTextItem *textLabel);

private:
    QString name;
    float frequency;
    QPointF *scenePosition = nullptr;
    QGraphicsRectItem *symbol = nullptr;
    QGraphicsSimpleTextItem *label = nullptr;

};

#endif // ATCBEACONVOR_H
