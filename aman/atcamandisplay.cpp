
#include "atcamandisplay.h"


ATCAmanDisplay::ATCAmanDisplay(QWidget *parent) : QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::NoDrag);
    setRenderHint(QPainter::Antialiasing);

    double sceneX = - ATCConst::AMAN_DISPLAY_WIDTH / 2;
    double sceneY = - ATCConst::AMAN_DISPLAY_HEIGHT / 2;
    double sceneW = ATCConst::AMAN_DISPLAY_WIDTH;
    double sceneH = ATCConst::AMAN_DISPLAY_HEIGHT;

    setSceneRect(sceneX, sceneY, sceneW, sceneH);

    currentScene = new QGraphicsScene(this);
    setScene(currentScene);
}

ATCAmanDisplay::~ATCAmanDisplay()
{
    for(int i = 0; i < flightLabels.size(); i++)
    {
        if(flightLabels.at(i) != nullptr) delete flightLabels.at(i);
    }
}

void ATCAmanDisplay::setSettings(ATCSettings *s)
{
    settings = s;
}

void ATCAmanDisplay::createTimeline(QTime *t)
{
    if(!timelineCreated)
    {
        //Set clock and lowest label
        time = t;
        lowestLabel = time->minute() - time->minute() % 5;

        //Set pen colours
        QPen timelinePen(settings->AMAN_TIMELINE_COLOR);
        timelinePen.setWidthF(settings->AMAN_TIMELINE_LINE_WIDTH);

        QPen horizonPen(settings->AMAN_HORIZON_COLOR);
        horizonPen.setWidthF(settings->AMAN_HORIZON_LINE_WIDTH);

        //Time scale vertical lines
        leftBar = new QGraphicsLineItem(-settings->AMAN_BAR_SPAN/2, -ATCConst::AMAN_DISPLAY_HEIGHT/2, -settings->AMAN_BAR_SPAN/2, ATCConst::AMAN_DISPLAY_HEIGHT);
        leftBar->setPen(timelinePen);
        currentScene->addItem(leftBar);

        rightBar = new QGraphicsLineItem(settings->AMAN_BAR_SPAN/2, -ATCConst::AMAN_DISPLAY_HEIGHT/2, settings->AMAN_BAR_SPAN/2, ATCConst::AMAN_DISPLAY_HEIGHT);
        rightBar->setPen(timelinePen);
        currentScene->addItem(rightBar);

        //Time scale ticks spacing
        majorTickSpacing = ATCConst::AMAN_DISPLAY_HEIGHT / 13;
        minorTickSpacing = majorTickSpacing / 5;

        //Time scale horizon
        QGraphicsLineItem *timeHorizon = new QGraphicsLineItem(-ATCConst::AMAN_DISPLAY_WIDTH/2, ATCConst::AMAN_DISPLAY_HEIGHT/2 - majorTickSpacing, ATCConst::AMAN_DISPLAY_WIDTH/2, ATCConst::AMAN_DISPLAY_HEIGHT/2 - majorTickSpacing);
        timeHorizon->setPen(horizonPen);
        currentScene->addItem(timeHorizon);

        //Time scale ticks
        double innerMajorTickBound = settings->AMAN_MAJOR_INNER_SPAN / 2;
        double outerMajorTickBound = settings->AMAN_MAJOR_OUTER_SPAN / 2;

        double innerMinorTickBound = settings->AMAN_MINOR_INNER_SPAN / 2;
        double outerMinorTickBound = settings->AMAN_MINOR_OUTER_SPAN / 2;

        double majorY = ATCConst::AMAN_DISPLAY_HEIGHT / 2;
        int labelValue = lowestLabel - 5;

        for(int i = 0; i < 15; i++)
        {
            //Create major tick
            QGraphicsLineItem *leftMajorTick = new QGraphicsLineItem(-outerMajorTickBound,  majorY, -innerMajorTickBound, majorY);
            leftMajorTick->setPen(timelinePen);
            currentScene->addItem(leftMajorTick);
            majorLeftTicks.append(leftMajorTick);

            QGraphicsLineItem *rightMajorTick = new QGraphicsLineItem(outerMajorTickBound, majorY, innerMajorTickBound, majorY);
            rightMajorTick->setPen(timelinePen);
            currentScene->addItem(rightMajorTick);
            majorRightTicks.append(rightMajorTick);

            //Create major tick label
            if((i > 0) && (i < 14))
            {
                QGraphicsSimpleTextItem *label = new QGraphicsSimpleTextItem(QString::number(labelValue).rightJustified(2, '0'));
                QBrush textBrush(settings->AMAN_TIMELINE_LABEL_COLOR);
                QFont textFont("Consolas");
                textFont.setPointSizeF(settings->AMAN_TIMELINE_LABEL_HEIGHT);

                label->setBrush(textBrush);
                label->setFont(textFont);

                double width = label->boundingRect().width();
                double height = label->boundingRect().height();

                label->setPos(-width/2, majorY - height / 2);

                currentScene->addItem(label);
                labels.append(label);
            }

            //Create minor ticks
            if(i < 14)
            {
                for(int j = 0; j < 4; j++)
                {
                    double minorY = majorY - (j + 1) * minorTickSpacing;

                    QGraphicsLineItem *leftMinorTick = new QGraphicsLineItem(-outerMinorTickBound, minorY, -innerMinorTickBound, minorY);
                    leftMinorTick->setPen(timelinePen);
                    currentScene->addItem(leftMinorTick);
                    minorLeftTicks.append(leftMinorTick);

                    QGraphicsLineItem *rightMinorTick = new QGraphicsLineItem(outerMinorTickBound, minorY, innerMinorTickBound, minorY);
                    rightMinorTick->setPen(timelinePen);
                    currentScene->addItem(rightMinorTick);
                    minorRightTicks.append(rightMinorTick);
                }
            }

            //Progress counters
            majorY -= majorTickSpacing;

            if(labelValue == 55)
            {
                labelValue = 0;
            }
            else labelValue += 5;
        }

        //Set timeline at correct time
        initializeTimelinePosition();

        //Set flag
        timelineCreated = true;
    }
}

void ATCAmanDisplay::appendFlightLabel(ATCAmanFlightLabel *lbl)
{
    flightLabels.append(lbl);
}

void ATCAmanDisplay::setLineEditMeteringFixVisible(bool flag)
{
    lineEditMeteringFixVisible = flag;
}

void ATCAmanDisplay::clockUpdated()
{
    //Progress time
    double dt = 1;
    progressTimeBy(dt);

    //Manage labels to create the proper illusion of timeline moving down all the time
    overflowCounter += dt;
    if(overflowCounter == 300)
    {
        progressTimeBy(-300);
        overflowCounter = 0;

        if(lowestLabel == 55)
        {
            lowestLabel = 0;
        }
        else lowestLabel += 5;


        int labelValue = lowestLabel;
        for(int i = 0; i < labels.size(); i++)
        {
            labels.at(i)->setText(QString::number(labelValue).rightJustified(2, '0'));

            if(labelValue == 55)
            {
                labelValue = 0;
            }
            else labelValue += 5;
        }
    }

    //Move flight labels down
    double dy = dt * minorTickSpacing / 60;

    for(int i = 0; i < flightLabels.size(); i++)
    {
        flightLabels.at(i)->moveByInterval(0, dy);
    }
}

void ATCAmanDisplay::slotFlightLabelSelected(ATCAmanFlightLabel *label)
{
    if(activeLabel != nullptr) activeLabel->deselect();
    activeLabel = label;
}

void ATCAmanDisplay::slotLabelHovered(bool flag)
{
    labelHovered = flag;
}

void ATCAmanDisplay::initializeTimelinePosition()
{
    int minutes = time->minute();
    int seconds = time->second();

    overflowCounter = (minutes % 5) * 60 + seconds;
    progressTimeBy(overflowCounter);
}

void ATCAmanDisplay::progressTimeBy(double seconds)
{
    double dy = seconds * minorTickSpacing / 60;

    for(int i = 0; i < minorLeftTicks.size(); i++)
    {
        minorLeftTicks.at(i)->moveBy(0, dy);
        minorRightTicks.at(i)->moveBy(0, dy);
    }

    for(int i = 0; i < majorRightTicks.size(); i++)
    {
        majorLeftTicks.at(i)->moveBy(0, dy);
        majorRightTicks.at(i)->moveBy(0, dy);
    }

    for(int i = 0; i < labels.size(); i++) labels.at(i)->moveBy(0, dy);
}

void ATCAmanDisplay::mousePressEvent(QMouseEvent *event)
{
    if(lineEditMeteringFixVisible)
    {
        emit signalHideLineEdit();
        lineEditMeteringFixVisible = false;
    }

    if(event->button() == Qt::LeftButton)
    {
        if((activeLabel != nullptr) && !activeLabel->isHovered())
        {
            emit activeLabel->signalFlightLabelSelected(nullptr);
        }
    }

    QGraphicsView::mousePressEvent(event);
    event->accept();
}
