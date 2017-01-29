
#include "atcclock.h"

ATCClock::ATCClock(QWidget *parent) : QPushButton(parent)
{
    setup();

    time = new QTime();
    time->start();
    setText(time->toString("HH:mm:ss"));

    timer = new QTimer(this);
    timer->setTimerType(Qt::CoarseTimer);
    timer->start(1000);

    connect(timer, SIGNAL(timeout()), this, SLOT(slotUpdateTime()));
}

ATCClock::~ATCClock()
{
    if(time != nullptr) delete time;
    if(timer != nullptr) delete timer;
}

QTime* ATCClock::getTime()
{
    return time;
}

void ATCClock::slotUpdateTime()
{
    *time = time->addSecs(1);
    setText(time->toString("HH:mm:ss"));
}

void ATCClock::setup()
{
    setEnabled(false);
    setStyleSheet(
                    "ATCClock"
                    "{"
                    "   color: #c8c8c8;"
                    "   font: 15px;"
                    "   background-color: #000000;"
                    "   border-style: outset;"
                    "   border-width: 2px;"
                    "   border-color: #3e3e3e;"
                    "}"
                );
}
