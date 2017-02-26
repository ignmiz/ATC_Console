
#include "atctrailingdot.h"

ATCTrailingDot::ATCTrailingDot(double xMid, double yMid, double *currentScale, ATCSettings *settings) :
    QGraphicsEllipseItem(xMid - settings->TRAILING_DIA / *currentScale / 2, yMid - settings->TRAILING_DIA / *currentScale / 2, settings->TRAILING_DIA / *currentScale, settings->TRAILING_DIA / *currentScale),
    settings(settings),
    position(xMid, yMid),
    currentScale(currentScale)
{
    setup();
}

ATCTrailingDot::ATCTrailingDot(QPointF &mid, double *currentScale, ATCSettings *settings) :
    QGraphicsEllipseItem(mid.x() - settings->TRAILING_DIA / *currentScale / 2, mid.y() - settings->TRAILING_DIA / *currentScale / 2, settings->TRAILING_DIA / *currentScale, settings->TRAILING_DIA / *currentScale),
    settings(settings),
    position(mid),
    currentScale(currentScale)
{
    setup();
}

ATCTrailingDot::~ATCTrailingDot()
{
}

void ATCTrailingDot::rescaleDot()
{
    double dia = settings->TRAILING_DIA / *currentScale;
    setRect(position.x() - dia/2, position.y() - dia/2, dia, dia);
}

void ATCTrailingDot::setup()
{
    setBrush(QBrush(settings->TRAILING_COLOR));
}

