#ifndef ATCTRAILINGDOT_H
#define ATCTRAILINGDOT_H

#include "atcsettings.h"

#include <QGraphicsEllipseItem>
#include <QDebug>

class ATCTrailingDot : public QGraphicsEllipseItem
{
public:
    explicit ATCTrailingDot(double xMid, double yMid, double *currentScale, ATCSettings *settings);
    explicit ATCTrailingDot(QPointF &mid, double *currentScale, ATCSettings *settings);
    ~ATCTrailingDot();

    void rescaleDot();

private:
    ATCSettings *settings;
    QPointF position;

    double *currentScale;

    void setup();

};

#endif // ATCTRAILINGDOT_H
