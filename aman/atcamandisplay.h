#ifndef ATCAMANDISPLAY_H
#define ATCAMANDISPLAY_H

#include <QMouseEvent>
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
    bool lineEditMeteringFixVisible;

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // ATCAMANDISPLAY_H
