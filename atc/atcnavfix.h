#ifndef ATCNAVFIX_H
#define ATCNAVFIX_H

#include <QGeoCoordinate>
#include <QGraphicsItem>

class ATCNavFix : public QGeoCoordinate
{
public:
    explicit ATCNavFix(QString fixName, double latitude, double longitude);
    ~ATCNavFix();

    QString getName();
    QGraphicsPolygonItem* getSymbol();
    QPointF* getScenePosiiton();
    QGraphicsSimpleTextItem* getLabel();

    void setName(QString fixName);
    void setSymbol(QGraphicsPolygonItem *fixSymbol);
    void setScenePosition(QPointF *position);
    void setLabel(QGraphicsSimpleTextItem *textLabel);

private:
    QString name;
    QPointF *scenePosition;
    QGraphicsPolygonItem *symbol;
    QGraphicsSimpleTextItem *label;
};

#endif // ATCNAVFIX_H
