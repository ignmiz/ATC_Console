
#include "atcamandisplay.h"


ATCAmanDisplay::ATCAmanDisplay(QWidget *parent) : QGraphicsView(parent)
{

}

ATCAmanDisplay::~ATCAmanDisplay()
{

}

void ATCAmanDisplay::setLineEditMeteringFixVisible(bool flag)
{
    lineEditMeteringFixVisible = flag;
}

void ATCAmanDisplay::mousePressEvent(QMouseEvent *event)
{
    if(lineEditMeteringFixVisible)
    {
        emit signalHideLineEdit();
        lineEditMeteringFixVisible = false;
    }

    QGraphicsView::mousePressEvent(event);
    event->accept();
}
