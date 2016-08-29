#ifndef ATCABSTRACTBEACON_H
#define ATCABSTRACTBEACON_H

#include <QGeoCoordinate>
#include <QGraphicsItem>
#include <QPen>

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
    void setColor(QColor color);

    void show();
    void hide();
    bool isVisible();
    void setFlagVisible(bool flag); //TEMPORARY

private:
    QString name;
    float frequency;
    QPointF *scenePosition = nullptr;
    QAbstractGraphicsShapeItem *symbol = nullptr;
    QGraphicsSimpleTextItem *label = nullptr;

    bool visible = false;
};

#endif // ATCABSTRACTBEACON_H
