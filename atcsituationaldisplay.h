#ifndef ATCSITUATIONALDISPLAY_H
#define ATCSITUATIONALDISPLAY_H

#include <QGraphicsView>

class ATCSituationalDisplay : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ATCSituationalDisplay(QWidget *parent = 0);
    ~ATCSituationalDisplay();

    qreal getBaseScale() const;
//    QPointF getMouseDragPosition() const;

    void setBaseScale(qreal scale);
//    void setMouseDragPosition(QMouseEvent *event);

private:
    qreal baseScale = 1;
    qreal scaleResolution = 0.1;
//    QPointF mouseDragPosition;

protected:
//    void scrollContentsBy(int dx, int dy);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // ATCSITUATIONALDISPLAY_H
