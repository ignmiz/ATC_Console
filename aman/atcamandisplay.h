#ifndef ATCAMANDISPLAY_H
#define ATCAMANDISPLAY_H

#include "atcconst.h"

#include <QMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsView>


class ATCAmanDisplay : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ATCAmanDisplay(QWidget *parent = 0);
    ~ATCAmanDisplay();

    void setLineEditMeteringFixVisible(bool flag);

signals:
    void signalHideLineEdit();

private:
    QGraphicsScene *currentScene;

    bool lineEditMeteringFixVisible;

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // ATCAMANDISPLAY_H
