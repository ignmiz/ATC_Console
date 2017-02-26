#ifndef ATCTRAILINGDOT_H
#define ATCTRAILINGDOT_H

#include "atcsettings.h"

#include <QGraphicsEllipseItem>

class ATCTrailingDot : public QGraphicsEllipseItem
{
public:
    explicit ATCTrailingDot(double xMid, double yMid, double *currentScale, ATCSettings *settings);
    ~ATCTrailingDot();

    void rescaleDot();

private:
    ATCSettings *settings;
    QPointF position;

    double *currentScale;

};

#endif // ATCTRAILINGDOT_H
