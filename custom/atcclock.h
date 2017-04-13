#ifndef ATCCLOCK_H
#define ATCCLOCK_H

#include <QPushButton>
#include <QTime>
#include <QTimer>
#include <QVector>

class ATCClock : public QPushButton
{
    Q_OBJECT

public:
    explicit ATCClock(QWidget *parent = 0);
    ~ATCClock();

    QTime* getTime();

    void start();
    void stop();

    void appendChildClock(QPushButton *child);

    void removeChildClock(int i);
    void removeChildClock(QPushButton *child);

private slots:
    void slotUpdateTime();

private:
    void setup();

    QTimer *timer;
    QTime *time;

    QVector<QPushButton*> childClocks;
};

#endif // ATCCLOCK_H
