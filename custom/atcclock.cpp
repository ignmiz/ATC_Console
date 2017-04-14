
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

void ATCClock::start()
{
    if(!timer->isActive())
    {
        timer->start(1000);
        connect(timer, SIGNAL(timeout()), this, SLOT(slotUpdateTime()));
    }
}

void ATCClock::stop()
{
    if(timer->isActive())
    {
        timer->stop();
        disconnect(timer, SIGNAL(timeout()), this, SLOT(slotUpdateTime()));
    }
}

void ATCClock::appendChildClock(QPushButton *child)
{
    child->setText(time->toString("HH:mm:ss"));
    childClocks.append(child);
}

void ATCClock::removeChildClock(int i)
{
    childClocks.remove(i);
}

void ATCClock::removeChildClock(QPushButton *child)
{

    for(int i = 0; i < childClocks.size(); i++)
    {
        if(childClocks.at(i) == child)
        {
            childClocks.remove(i);
            break;
        }
    }
}

void ATCClock::slotUpdateTime()
{
    *time = time->addSecs(1);
    setText(time->toString("HH:mm:ss"));

    emit signalClockUpdated();

    //Update child clocks
    for(int i = 0; i < childClocks.size(); i++)
    {
        childClocks.at(i)->setText(time->toString("HH:mm:ss"));
    }
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
