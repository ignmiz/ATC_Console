#ifndef ATCAMANDISPLAY_H
#define ATCAMANDISPLAY_H

#include "atcconst.h"

#include <QList>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QDebug>

class ATCAmanDisplay : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ATCAmanDisplay(QWidget *parent = 0);
    ~ATCAmanDisplay();

    void setLineEditMeteringFixVisible(bool flag);
    void clockUpdated();

signals:
    void signalHideLineEdit();

private:
    QGraphicsScene *currentScene;

    QGraphicsLineItem *leftBar;
    QGraphicsLineItem *rightBar;
    QList<QGraphicsLineItem*> minorLeftTicks;
    QList<QGraphicsLineItem*> minorRightTicks;
    QList<QGraphicsLineItem*> majorLeftTicks;
    QList<QGraphicsLineItem*> majorRightTicks;
    QList<QGraphicsSimpleTextItem*> labels;

    double majorTickSpacing;
    double minorTickSpacing;

    bool lineEditMeteringFixVisible;

    void progressTimeBy(double seconds);

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // ATCAMANDISPLAY_H
