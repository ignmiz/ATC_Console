#ifndef ATCNAVFIX_H
#define ATCNAVFIX_H

#include "atcabstractfix.h"

#include <QGraphicsItem>
#include <QPen>

class ATCNavFix : public ATCAbstractFix
{
public:
    explicit ATCNavFix(QString fixName, double latitude, double longitude);
    ~ATCNavFix();

    QString getName();
    QAbstractGraphicsShapeItem* getSymbol();
    QPointF* getScenePosition();
    QGraphicsSimpleTextItem* getLabel();

    void setName(QString fixName);
    void setSymbol(QAbstractGraphicsShapeItem *fixSymbol);
    void setScenePosition(QPointF *position);
    void setLabel(QGraphicsSimpleTextItem *textLabel);
    void setColor(QColor color);

    void show();
    void hide();
    bool isVisible();

private:
    QString name;
    QPointF *scenePosition = nullptr;
    QAbstractGraphicsShapeItem *symbol = nullptr;
    QGraphicsSimpleTextItem *label = nullptr;

    bool visible = false;
};

#endif // ATCNAVFIX_H
