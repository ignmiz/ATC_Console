
#include "atctrailingdot.h"

ATCTrailingDot::ATCTrailingDot(double xMid, double yMid, double *currentScale, ATCSettings *settings) :
    settings(settings),
    position(xMid, yMid),
    currentScale(currentScale),
    QGraphicsEllipseItem(xMid - settings->TRAILING_DIA / *currentScale / 2, yMid - settings->TRAILING_DIA / *currentScale / 2, settings->TRAILING_DIA / *currentScale, settings->TRAILING_DIA / *currentScale)
{
    setBrush(settings->TRAILING_COLOR);
}

ATCTrailingDot::~ATCTrailingDot()
{

}

void ATCTrailingDot::rescaleDot()
{
    double dia = settings->TRAILING_DIA / *currentScale;
    setRect(position.x() - dia/2, position.y() - dia/2, dia, dia);
}

