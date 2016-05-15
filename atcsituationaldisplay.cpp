#include "atcsituationaldisplay.h"

#include <QWheelEvent>
#include <QCursor>
#include <QGuiApplication>
#include <QDebug>

ATCSituationalDisplay::ATCSituationalDisplay(QWidget *parent) : QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
//    setTransformationAnchor(QGraphicsView::NoAnchor);
    this->setDragMode(QGraphicsView::NoDrag);
//    this->setAlignment();
    this->setSceneRect(-50000, -50000, 100000, 100000);

    viewport()->setCursor(Qt::CrossCursor);
}

ATCSituationalDisplay::~ATCSituationalDisplay()
{

}

qreal ATCSituationalDisplay::getBaseScale() const
{
    return baseScale;
}

//QPoint ATCSituationalDisplay::getMouseDragPosition() const
//{
//    return mouseDragPosition;
//}

void ATCSituationalDisplay::setBaseScale(qreal scale)
{
    baseScale = scale;
}

//void ATCSituationalDisplay::setMouseDragPosition(QMouseEvent *event)
//{
//    mouseDragPosition = mapToScene(event->globalPos() - frameGeometry().topLeft());
//    qDebug() << mouseDragPosition;
//}

//void ATCSituationalDisplay::scrollContentsBy(int dx, int dy)
//{
//}

void ATCSituationalDisplay::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta();

    qreal newScale = baseScale + (numDegrees.y() / 120) * scaleResolution;
    this->scale(newScale, newScale);

    event->accept();
}

void ATCSituationalDisplay::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
}

void ATCSituationalDisplay::mouseMoveEvent(QMouseEvent *event)
{
    if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier))
    {
        QGraphicsView::mouseMoveEvent(event);
    }
    event->accept();
}

void ATCSituationalDisplay::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
}

void ATCSituationalDisplay::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        this->setDragMode(QGraphicsView::ScrollHandDrag);        
    }
    event->accept();
}

void ATCSituationalDisplay::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        this->setDragMode(QGraphicsView::NoDrag);
        viewport()->setCursor(Qt::CrossCursor);        
    }
    event->accept();
}
