#include "dialogaman.h"
#include "ui_dialogaman.h"

DialogAman::DialogAman(ATCAirspace *airspace, ATCSettings *settings, QTime *time, QWidget *parent) :
    ATCDialog(parent, "Approach Manager", 700, 900),
    airspace(airspace),
    settings(settings),
    time(time),
    uiInner(new Ui::DialogAman)
{
    uiInner->setupUi(this);
    windowSetup();

    uiInner->amanDisplay->setSettings(settings);
    uiInner->amanDisplay->createTimeline(time);

    deactivateRTAgui();
    toggleClearAll();

    updateStatsEtiquette();

    QFont font("Consolas");
    font.setPointSizeF(12);
    uiInner->labelStats->setFont(font);

    createLineEdit();
    connect(uiInner->amanDisplay, SIGNAL(signalHideLineEdit()), this, SLOT(slotHideLineEdit()));
    connect(uiInner->amanDisplay, SIGNAL(signalScrollBy(int)), this, SLOT(slotScrollBy(int)));

    connect(uiInner->horizontalSlider, SIGNAL(sliderPressed()), this, SLOT(slotSliderPressed()));
    connect(uiInner->horizontalSlider, SIGNAL(sliderReleased()), this, SLOT(slotSliderReleased()));
}

DialogAman::~DialogAman()
{
    if(lineEditMeteringFix != nullptr) delete lineEditMeteringFix;

    QHash<ATCFlight*, ATCAmanFlightLabel*>::iterator i;
    for(i = labelsHash.begin(); i != labelsHash.end(); i++)
    {
        if(i.value() != nullptr) delete i.value();
    }

    delete uiInner;
}

QPushButton *DialogAman::getClock()
{
    return uiInner->buttonClock;
}

void DialogAman::setMeteringFix(QString &fix)
{
    if(fix.isEmpty() || airspace->isValidNavaid(fix)) uiInner->buttonMeteringFix->setText(fix);
}

void DialogAman::setSimulation(ATCSimulation *sim)
{
    simulation = sim;
}

void DialogAman::countRTAs()
{
    QHash<ATCFlight*, ATCAmanFlightLabel*>::iterator i;
    RTAcount = 0;

    for(i = labelsHash.begin(); i != labelsHash.end(); i++)
    {
        if(i.key()->getRTA().isValid()) RTAcount++;
    }
}

void DialogAman::on_buttonMeteringFix_clicked()
{
    uiInner->buttonMeteringFix->hide();

    lineEditMeteringFix->setText(uiInner->buttonMeteringFix->text());
    lineEditMeteringFix->setStyleSheet("QLineEdit"
                                       "{"
                                       "    color: #c8c8c8;"
                                       "    background-color: #00153a;"
                                       "    border-style: inset;"
                                       "    border-width: 2px;"
                                       "    border-color: #3e3e3e;"
                                       "    font: 14px;"
                                       "}"
                                       );
    lineEditMeteringFix->selectAll();
    lineEditMeteringFix->setFocus();
    lineEditMeteringFix->show();

    uiInner->amanDisplay->setLineEditMeteringFixVisible(true);
}

void DialogAman::on_buttonSetRTA_clicked()
{
    if(activeLabel != nullptr)
    {
        if(!activeLabel->getFlight()->getRTA().isValid()) RTAcount++;

        activeLabel->getFlight()->setRTA(uiInner->timeEdit->time());
        activeLabel->updateRTA();
        activeLabel->updateColor();

        slotUpdateStatistics();

        emit activeLabel->signalFlightLabelSelected(nullptr);
        toggleClearAll();
    }
}

void DialogAman::on_buttonClear_clicked()
{
    if(activeLabel != nullptr)
    {
        activeLabel->getFlight()->setRTA(QTime());
        activeLabel->updateRTA();
        activeLabel->updateColor();

        emit activeLabel->signalFlightLabelSelected(nullptr);

        RTAcount--;
        slotUpdateStatistics();

        toggleClearAll();
    }
}

void DialogAman::on_buttonClearAll_clicked()
{
    if(activeLabel != nullptr) emit activeLabel->signalFlightLabelSelected(nullptr);

    QHash<ATCFlight*, ATCAmanFlightLabel*>::iterator i;
    for(i = labelsHash.begin(); i != labelsHash.end(); i++)
    {
        ATCAmanFlightLabel *current = i.value();

        if(current->getFlight()->getRTA().isValid())
        {
            current->getFlight()->setRTA(QTime());
            current->updateRTA();
            current->updateColor();
        }
    }

    RTAcount = 0;
    slotUpdateStatistics();

    toggleClearAll();
}

void DialogAman::on_buttonClock_clicked()
{
    if(uiInner->amanDisplay->getPageNumber() > 0)
    {
        uiInner->amanDisplay->resetPageNumber();
    }
}

void DialogAman::slotMeteringFixEntered()
{
    QString name = lineEditMeteringFix->text();

    if(!name.isEmpty() && !airspace->isValidNavaid(name))
    {
        lineEditMeteringFix->setStyleSheet("QLineEdit"
                                           "{"
                                           "    color: #ff0000;"
                                           "    background-color: #00153a;"
                                           "    border-style: inset;"
                                           "    border-width: 2px;"
                                           "    border-color: #3e3e3e;"
                                           "    font: 14px;"
                                           "}"
                                          );
        return;
    }

    lineEditMeteringFix->hide();

    uiInner->buttonMeteringFix->setText(name);
    uiInner->buttonMeteringFix->show();

    QHash<ATCFlight*, ATCAmanFlightLabel*>::iterator i;
    for(i = labelsHash.begin(); i != labelsHash.end(); i++)
    {
        ATCAmanFlightLabel *label = i.value();
        if(label == activeLabel) emit activeLabel->signalFlightLabelSelected(nullptr);
        if(label->isOnScene()) uiInner->amanDisplay->removeFlightLabel(label);
        delete label;
    }

    labelsHash.clear();

    RTAcount = 0;
    onTimeCount = 0;
    deviatedCount = 0;
    compromisedCount = 0;

    updateStatsEtiquette();

    if(simulation != nullptr) simulation->setMeteringFix(name);
}

void DialogAman::slotHideLineEdit()
{
    lineEditMeteringFix->hide();
    lineEditMeteringFix->clear();


    uiInner->buttonMeteringFix->show();
}

void DialogAman::slotClockUpdated()
{
    uiInner->amanDisplay->clockUpdated();

    if(activeLabel != nullptr)
    {
        if(activeLabel->getRangeBar() != nullptr && activeLabel->getSelector() != nullptr)
        {
            updateTimeRangeBar();
            initializeTimeEditValue();
        }
    }
}

void DialogAman::slotFlightLabelSelected(ATCAmanFlightLabel *label)
{
    //Disconnect previous activeLabel
    if(activeLabel != nullptr)
    {
        disconnect(uiInner->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
        disconnect(uiInner->timeEdit, SIGNAL(timeChanged(QTime)), this, SLOT(slotTimeChanged(QTime)));
    }

    //Assign new active label
    activeLabel = label;

    //Perform actions based on whether label exists
    if(label != nullptr)
    {
        if(!RTAgui)
        {
            createTimeRangeBar();
            createSelector();

            activateRTAgui(label->getFlight()->getRTA().isValid());
            initializeSliderPosition();
            initializeTimeEditValue();
        }

        connect(uiInner->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
        connect(uiInner->timeEdit, SIGNAL(timeChanged(QTime)), this, SLOT(slotTimeChanged(QTime)));
    }
    else
    {
        if(RTAgui) deactivateRTAgui();
    }
}

void DialogAman::slotValueChanged(int value)
{
    if(activeLabel != nullptr)
    {
        //Set selector position
        QGraphicsRectItem *rangeBar = activeLabel->getRangeBar();
        QGraphicsLineItem *selector = activeLabel->getSelector();

        double rangeBarBottom = rangeBar->mapToScene(rangeBar->rect().bottomLeft()).y();
        double rangeBarUpper = rangeBar->mapToScene(rangeBar->rect().topLeft()).y();

        double selectorPosition = rangeBarUpper + static_cast<double>(value) / 100 * (rangeBarBottom - rangeBarUpper);

        QLineF line = selector->line();

        QPointF inner(line.p1().x(), selectorPosition);
        QPointF outer(line.p2().x(), selectorPosition);

        selector->setLine(QLineF(selector->mapFromScene(inner), selector->mapFromScene(outer)));

        //Set time edit value
        uiInner->timeEdit->setTime(timeFromY(selectorPosition));
    }
}

void DialogAman::slotTimeChanged(QTime t)
{
    if((activeLabel != nullptr) && !flagSliderPressed)
    {
        //Set selector position
        QGraphicsLineItem *selector = activeLabel->getSelector();
        QLineF line = selector->line();

        double y = timeToY(t);

        QPointF p1new = selector->mapFromScene(line.p1().x(), y);
        QPointF p2new = selector->mapFromScene(line.p2().x(), y);

        selector->setLine(QLineF(p1new, p2new));

        //Set slider value
        double lowBound = ATCConst::AMAN_DISPLAY_HEIGHT / 2;

        QGraphicsRectItem *rangeBar = activeLabel->getRangeBar();

        double selectorDst = lowBound - selector->mapToScene(selector->line().p1()).y();
        double rangeBarBottomDst = lowBound - rangeBar->mapToScene(rangeBar->rect().bottomLeft()).y();
        double rangeBarUpperDst = lowBound - rangeBar->mapToScene(rangeBar->rect().topLeft()).y();

        int sliderValue = qRound((rangeBarUpperDst - selectorDst) / (rangeBarUpperDst - rangeBarBottomDst) * 100);

        uiInner->horizontalSlider->setValue(sliderValue);
    }
}

void DialogAman::slotSliderPressed()
{
    flagSliderPressed = true;
}

void DialogAman::slotSliderReleased()
{
    flagSliderPressed = false;
}

void DialogAman::slotMeteringFixFound(ATCFlight *flight)
{
    if(!uiInner->buttonMeteringFix->text().isEmpty())
    {
        QHash<ATCFlight*, ATCAmanFlightLabel*>::iterator i = labelsHash.find(flight);
        if(i == labelsHash.end()) //Flight not found in container - create
        {
            QTime ETA = flight->getWaypointTime(flight->getMeteringFixIndex());

            ATCAmanFlightLabel *label = new ATCAmanFlightLabel(flight, time, QPointF(32, timeToY(ETA)));
            label->setPageNumber(uiInner->amanDisplay->getPageNumberPointer());
            label->addToScene(uiInner->amanDisplay->scene());
            uiInner->amanDisplay->insertFlightLabel(label);

            labelsHash.insert(flight, label);

            connect(label, SIGNAL(signalFlightLabelSelected(ATCAmanFlightLabel*)), uiInner->amanDisplay, SLOT(slotFlightLabelSelected(ATCAmanFlightLabel*)));
            connect(label, SIGNAL(signalFlightLabelSelected(ATCAmanFlightLabel*)), this, SLOT(slotFlightLabelSelected(ATCAmanFlightLabel*)));
            connect(label, SIGNAL(signalLabelHovered(bool)), uiInner->amanDisplay, SLOT(slotLabelHovered(bool)));

            if(flight->getRTA().isValid()) RTAcount++;
        }
        else //Flight found in container - update
        {
            i.value()->updateEtiquette();
            i.value()->updateColor();
            updateLabelPosition(i.value());
        }

        slotUpdateStatistics();
        toggleClearAll();
    }
}

void DialogAman::slotMeteringFixLost(ATCFlight *flight)
{
    if(!uiInner->buttonMeteringFix->text().isEmpty())
    {
        QHash<ATCFlight*, ATCAmanFlightLabel*>::iterator i = labelsHash.find(flight);

        if(i != labelsHash.end())
        {
            ATCAmanFlightLabel *label = i.value();

            if(label != nullptr)
            {
                if(label == activeLabel) emit activeLabel->signalFlightLabelSelected(nullptr);
                if(label->isOnScene()) uiInner->amanDisplay->removeFlightLabel(label);
                delete label;
            }

            labelsHash.erase(i);

            if(flight->getRTA().isValid())
            {
                flight->setRTA(QTime());
                RTAcount--;
            }

            slotUpdateStatistics();
            toggleClearAll();
        }
    }
}

void DialogAman::slotUpdateStatistics()
{
    onTimeCount = 0;
    deviatedCount = 0;
    compromisedCount = 0;

    QHash<ATCFlight*, ATCAmanFlightLabel*>::iterator i;
    for(i = labelsHash.begin(); i != labelsHash.end(); i++)
    {
        ATCAmanFlightLabel *label = i.value();

        if(label->isOnTime()) onTimeCount++;
        if(label->isDeviated()) deviatedCount++;
        if(label->isCompromised()) compromisedCount++;
    }

    updateStatsEtiquette();
}

void DialogAman::slotScrollBy(int i)
{
    double pageInterval = ATCConst::AMAN_DISPLAY_HEIGHT / 13;

    QHash<ATCFlight*, ATCAmanFlightLabel*>::iterator it;
    for(it = labelsHash.begin(); it != labelsHash.end(); it++)
    {
        it.value()->moveByInterval(0, i * pageInterval);
    }
}

void DialogAman::createLineEdit()
{
    lineEditMeteringFix = new QLineEdit(this);
    lineEditMeteringFix->setGeometry(290, 875, 120, 25);
    lineEditMeteringFix->setAlignment(Qt::AlignHCenter);
    lineEditMeteringFix->setInputMask(">nnnnn");
    lineEditMeteringFix->hide();

    connect(lineEditMeteringFix, SIGNAL(returnPressed()), this, SLOT(slotMeteringFixEntered()));
}

void DialogAman::updateStatsEtiquette()
{
    QString etiquette("CONCERNED: " + QString::number(labelsHash.size()).rightJustified(3, ' ') + "     ON TIME: " + QString::number(onTimeCount).rightJustified(3, ' ') + "\n"
                      "UNMETERED: " + QString::number(labelsHash.size() - RTAcount).rightJustified(3, ' ') + "    DEVIATED: " + QString::number(deviatedCount).rightJustified(3, ' ') + "\n"
                      "  METERED: " + QString::number(RTAcount).rightJustified(3, ' ') + " COMPROMISED: " + QString::number(compromisedCount).rightJustified(3, ' ')
                     );
    uiInner->labelStats->setText(etiquette);
}

void DialogAman::createSelector()
{
    QGraphicsLineItem *timeArrow = activeLabel->getTimeArrow();
    QPointF arrow = timeArrow->mapToScene(timeArrow->line().p1());

    activeLabel->createSelector(uiInner->amanDisplay->scene(), arrow.y());
}

void DialogAman::createTimeRangeBar()
{
    ATCFlight *flight = activeLabel->getFlight();
    QGraphicsLineItem *timeArrow = activeLabel->getTimeArrow();
    QPointF arrow = timeArrow->mapToScene(timeArrow->line().p1());

    double secsToMeteringFix = static_cast<double>(time->msecsTo(flight->getWaypointTime(flight->getMeteringFixIndex()))) / 1000;

    if(secsToMeteringFix > 0)
    {
        double oneSecondInterval = ATCConst::AMAN_DISPLAY_HEIGHT / 13 / 5 / 60;

        double dstToMeteringFix = flight->getDistanceToNext();;
        for(int i = flight->getWaypointIndex(); i < flight->getMeteringFixIndex(); i++)
        {
            dstToMeteringFix += flight->getLegDistance(i);
        }

        double avgSpdToMeteringFix = dstToMeteringFix / secsToMeteringFix;

        double lower = qFabs(dstToMeteringFix / (ATCConst::TRAJECTORY_SPD_INC * avgSpdToMeteringFix) - secsToMeteringFix);
        double upper = qFabs(dstToMeteringFix / (ATCConst::TRAJECTORY_SPD_DEC * avgSpdToMeteringFix) - secsToMeteringFix);

        double height = (lower + upper) * oneSecondInterval; //Lower part + upper part
        double topY = arrow.y() - upper * oneSecondInterval;

        activeLabel->createTimeRangeBar(uiInner->amanDisplay->scene(), topY, height);
    }
    else
    {
        activeLabel->createTimeRangeBar(uiInner->amanDisplay->scene(), arrow.y(), 0);
    }
}

void DialogAman::updateTimeRangeBar()
{
    ATCFlight *flight = activeLabel->getFlight();
    QGraphicsLineItem *timeArrow = activeLabel->getTimeArrow();
    QPointF arrow = timeArrow->mapToScene(timeArrow->line().p1());
    QGraphicsRectItem *rangeBar = activeLabel->getRangeBar();

    double secsToMeteringFix = static_cast<double>(time->msecsTo(flight->getWaypointTime(flight->getMeteringFixIndex()))) / 1000;

    if(secsToMeteringFix > 0)
    {
        double oneSecondInterval = ATCConst::AMAN_DISPLAY_HEIGHT / 13 / 5 / 60;

        double dstToMeteringFix = flight->getDistanceToNext();;
        for(int i = flight->getWaypointIndex(); i < flight->getMeteringFixIndex(); i++)
        {
            dstToMeteringFix += flight->getLegDistance(i);
        }

        double avgSpdToMeteringFix = dstToMeteringFix / secsToMeteringFix;

        qDebug() << ATCMath::m2nm(dstToMeteringFix) << secsToMeteringFix << ATCMath::mps2kt(avgSpdToMeteringFix);

        double lower = qFabs(dstToMeteringFix / (ATCConst::TRAJECTORY_SPD_INC * avgSpdToMeteringFix) - secsToMeteringFix);
        double upper = qFabs(dstToMeteringFix / (ATCConst::TRAJECTORY_SPD_DEC * avgSpdToMeteringFix) - secsToMeteringFix);

        double height = (lower + upper) * oneSecondInterval; //Lower part + upper part
        double topY = arrow.y() - upper * oneSecondInterval;

        rangeBar->setRect(rangeBar->mapRectFromScene(QRectF(-30, topY, 60, height)));
    }
    else
    {
        activeLabel->createTimeRangeBar(uiInner->amanDisplay->scene(), arrow.y(), 0);
    }
}

void DialogAman::activateRTAgui(bool hasRTA)
{
    uiInner->timeEdit->setEnabled(true);
    uiInner->buttonSetRTA->setEnabled(true);
    if(hasRTA) uiInner->buttonClear->setEnabled(true);
    uiInner->horizontalSlider->setEnabled(true);
    uiInner->labelLater->setEnabled(true);
    uiInner->labelSooner->setEnabled(true);

    RTAgui = true;
}

void DialogAman::deactivateRTAgui()
{
    uiInner->timeEdit->setEnabled(false);
    uiInner->buttonSetRTA->setEnabled(false);
    uiInner->buttonClear->setEnabled(false);
    uiInner->horizontalSlider->setEnabled(false);
    uiInner->labelLater->setEnabled(false);
    uiInner->labelSooner->setEnabled(false);

    uiInner->timeEdit->setMinimumTime(QTime(0, 0, 0));
    uiInner->timeEdit->setMaximumTime(QTime(23, 59, 59, 999));

    uiInner->timeEdit->setTime(QTime(0, 0, 0));
    uiInner->horizontalSlider->setValue(50);

    RTAgui = false;
}

void DialogAman::toggleClearAll()
{
    if(RTAcount > 0)
        uiInner->buttonClearAll->setEnabled(true);
    else
        uiInner->buttonClearAll->setEnabled(false);
}

QTime DialogAman::timeFromY(double y)
{
    double pageDelta = uiInner->amanDisplay->getPageDelta();
    int pageNumber = uiInner->amanDisplay->getPageNumber();

    double oneSecondInterval = ATCConst::AMAN_DISPLAY_HEIGHT / 13 / 5 / 60;
    double secondsFromCurrent = pageNumber * pageDelta + (ATCConst::AMAN_DISPLAY_HEIGHT / 2 - y) / oneSecondInterval - 300;

    return time->addMSecs(qRound(secondsFromCurrent * 1000));
}

double DialogAman::timeToY(QTime &t)
{
    double pageDelta = uiInner->amanDisplay->getPageDelta();
    int pageNumber = uiInner->amanDisplay->getPageNumber();

    double oneSecondInterval = ATCConst::AMAN_DISPLAY_HEIGHT / 13 / 5 / 60;
    double secDiff = static_cast<double>(time->msecsTo(t)) / 1000.0;

    return ATCConst::AMAN_DISPLAY_HEIGHT / 2 - (300.0 + secDiff - pageNumber * pageDelta) * oneSecondInterval;
}

void DialogAman::initializeSliderPosition()
{
    if(activeLabel != nullptr)
    {
        double lowBound = ATCConst::AMAN_DISPLAY_HEIGHT / 2;

        QGraphicsLineItem *selector = activeLabel->getSelector();
        QGraphicsRectItem *rangeBar = activeLabel->getRangeBar();

        double selectorDst = lowBound - selector->mapToScene(selector->line().p1()).y();
        double rangeBarBottomDst = lowBound - rangeBar->mapToScene(rangeBar->rect().bottomLeft()).y();
        double rangeBarUpperDst = lowBound - rangeBar->mapToScene(rangeBar->rect().topLeft()).y();

        int sliderValue = qRound((rangeBarUpperDst - selectorDst) / (rangeBarUpperDst - rangeBarBottomDst) * 100);

        uiInner->horizontalSlider->setValue(sliderValue);
    }
}

void DialogAman::initializeTimeEditValue()
{
    if(activeLabel != nullptr)
    {
        QGraphicsLineItem *selector = activeLabel->getSelector();
        uiInner->timeEdit->setTime(timeFromY(selector->mapToScene(selector->line().p1()).y()));

        QGraphicsRectItem *rangeBar = activeLabel->getRangeBar();
        uiInner->timeEdit->setMinimumTime(timeFromY(rangeBar->mapToScene(rangeBar->rect().bottomLeft()).y()));
        uiInner->timeEdit->setMaximumTime(timeFromY(rangeBar->mapToScene(rangeBar->rect().topLeft()).y()));
    }
}

void DialogAman::updateLabelPosition(ATCAmanFlightLabel *label)
{
    //Assign pointers
    ATCFlight *flight = label->getFlight();
    QGraphicsLineItem *timeArrow = label->getTimeArrow();

    //Assign times
    QTime previousETA = timeFromY(timeArrow->mapToScene(timeArrow->line().p1()).y());
    QTime ETA = flight->getWaypointTime(flight->getMeteringFixIndex());

    //Calculate time difference
    double secondsDiff = static_cast<double>(ETA.msecsTo(previousETA)) / 1000.0;
    double oneSecondInterval = ATCConst::AMAN_DISPLAY_HEIGHT / 13 / 5 / 60;

    //Move label
    label->moveByInterval(0, oneSecondInterval * secondsDiff);
}

bool DialogAman::isWithinSceneBoundaries(ATCAmanFlightLabel *label)
{
    QGraphicsLineItem *timeArrow = label->getTimeArrow();
    double y = timeArrow->mapToScene(timeArrow->line().p1()).y();

    double limit = ATCConst::AMAN_DISPLAY_HEIGHT / 2;

    if((y > limit) || (y < -limit)) return false;
    else return true;
}

