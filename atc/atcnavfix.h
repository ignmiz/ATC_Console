#ifndef ATCNAVFIX_H
#define ATCNAVFIX_H

#include "atcabstractfix.h"

#include <QGraphicsItem>

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

private:
    QString name;
    QPointF *scenePosition = nullptr;
    QAbstractGraphicsShapeItem *symbol = nullptr;
    QGraphicsSimpleTextItem *label = nullptr;
};

#endif // ATCNAVFIX_H
