#ifndef ATCCLOCK_H
#define ATCCLOCK_H

#include <QPushButton>
#include <QTime>
#include <QTimer>

class ATCClock : public QPushButton
{
    Q_OBJECT

public:
    explicit ATCClock(QWidget *parent = 0);
    ~ATCClock();

    QTime* getTime();

    void start();
    void stop();

private slots:
    void slotUpdateTime();

private:
    void setup();

    QTimer *timer;
    QTime *time;
};

#endif // ATCCLOCK_H
