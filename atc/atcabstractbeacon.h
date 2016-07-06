#ifndef ATCABSTRACTBEACON_H
#define ATCABSTRACTBEACON_H

#include <QGeoCoordinate>
#include <QGraphicsItem>

class ATCAbstractBeacon : public QGeoCoordinate
{
public:
    explicit ATCAbstractBeacon(QString beaconName, float beaconFrequency, double latitude, double longitude);
    virtual ~ATCAbstractBeacon() = 0;

    QString getName() const;
    float getFrequency() const;
    QAbstractGraphicsShapeItem* getSymbol() const;
    QPointF* getScenePosition() const;
    QGraphicsSimpleTextItem* getLabel() const;

    void setName(QString beaconName);
    void setFrequency(float beaconFrequency);
    void setSymbol(QAbstractGraphicsShapeItem *graphicsItem);
    void setScenePosition(QPointF *position);
    void setLabel(QGraphicsSimpleTextItem *textLabel);

private:
    QString name;
    float frequency;
    QPointF *scenePosition = nullptr;
    QAbstractGraphicsShapeItem *symbol = nullptr;
    QGraphicsSimpleTextItem *label = nullptr;
};

#endif // ATCABSTRACTBEACON_H
