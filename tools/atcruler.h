#ifndef ATCRULER_H
#define ATCRULER_H

#include "atcmath.h"
#include "atcsettings.h"

#include <QGraphicsItem>
#include <QGraphicsScene>

class ATCRuler
{
public:
    explicit ATCRuler(QPointF p1, ATCSettings *settings, QGraphicsScene *scene, double *currentScale);
    ~ATCRuler();

    void moveEnd(QPointF p2);

private:
    ATCSettings *settings;
    QGraphicsScene *scene;
    double *currentScale;

    QPointF p1;

    QGraphicsLineItem *line = nullptr;
    QGraphicsEllipseItem *circ1 = nullptr;
    QGraphicsEllipseItem *circ2 = nullptr;
    QGraphicsSimpleTextItem *text = nullptr;

    void constructRuler();
};

#endif // ATCRULER_H
