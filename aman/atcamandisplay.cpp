
#include "atcamandisplay.h"


ATCAmanDisplay::ATCAmanDisplay(QWidget *parent) : QGraphicsView(parent)
{
//    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    setDragMode(QGraphicsView::NoDrag);
    setRenderHint(QPainter::Antialiasing);

    double sceneX = - ATCConst::AMAN_DISPLAY_WIDTH / 2;
    double sceneY = - ATCConst::AMAN_DISPLAY_HEIGHT / 2;
    double sceneW = ATCConst::AMAN_DISPLAY_WIDTH;
    double sceneH = ATCConst::AMAN_DISPLAY_HEIGHT;

    setSceneRect(sceneX, sceneY, sceneW, sceneH);

    currentScene = new QGraphicsScene(this);
    setScene(currentScene);

    //----TEST----
    QPen pen(Qt::white);
    pen.setWidthF(2);

    QPen pen2(Qt::green);
    pen2.setWidthF(2);

    QPen pen3(Qt::red);
    pen3.setWidthF(1);

    //Time scale vertical lines
    leftBar = new QGraphicsLineItem(-30, -ATCConst::AMAN_DISPLAY_HEIGHT/2, -30, ATCConst::AMAN_DISPLAY_HEIGHT);
    leftBar->setPen(pen);
    currentScene->addItem(leftBar);

    rightBar = new QGraphicsLineItem(30, -ATCConst::AMAN_DISPLAY_HEIGHT/2, 30, ATCConst::AMAN_DISPLAY_HEIGHT);
    rightBar->setPen(pen);
    currentScene->addItem(rightBar);

    //Time scale ticks spacing
    majorTickSpacing = ATCConst::AMAN_DISPLAY_HEIGHT / 13;
    minorTickSpacing = majorTickSpacing / 5;

    //Time scale horizon
    QGraphicsLineItem *timeHorizon = new QGraphicsLineItem(-ATCConst::AMAN_DISPLAY_WIDTH/2, ATCConst::AMAN_DISPLAY_HEIGHT/2 - majorTickSpacing, ATCConst::AMAN_DISPLAY_WIDTH/2, ATCConst::AMAN_DISPLAY_HEIGHT/2 - majorTickSpacing);
    timeHorizon->setPen(pen3);
    currentScene->addItem(timeHorizon);

    //Time scale ticks
    double innerMajorTickBound = 15;
    double outerMajorTickBound = 30;

    double innerMinorTickBound = 25;
    double outerMinorTickBound = 30;

    double majorY = ATCConst::AMAN_DISPLAY_HEIGHT / 2 + majorTickSpacing;
    int labelValue = -10;

    for(int i = 0; i < 15; i++)
    {
        //Create major tick
        QGraphicsLineItem *leftMajorTick = new QGraphicsLineItem(-outerMajorTickBound,  majorY, -innerMajorTickBound, majorY);
        leftMajorTick->setPen(pen);
        currentScene->addItem(leftMajorTick);
        majorLeftTicks.append(leftMajorTick);

        QGraphicsLineItem *rightMajorTick = new QGraphicsLineItem(outerMajorTickBound, majorY, innerMajorTickBound, majorY);
        rightMajorTick->setPen(pen);
        currentScene->addItem(rightMajorTick);
        majorRightTicks.append(rightMajorTick);

        //Create major tick label
        if((i > 1) && (i < 14))
        {
            QGraphicsSimpleTextItem *label = new QGraphicsSimpleTextItem(QString::number(labelValue).rightJustified(2, '0'));
            QBrush textBrush(Qt::white);
            QFont textFont("Consolas");
            textFont.setPointSizeF(12);

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
                leftMinorTick->setPen(pen);
                currentScene->addItem(leftMinorTick);
                minorLeftTicks.append(leftMinorTick);

                QGraphicsLineItem *rightMinorTick = new QGraphicsLineItem(outerMinorTickBound, minorY, innerMinorTickBound, minorY);
                rightMinorTick->setPen(pen);
                currentScene->addItem(rightMinorTick);
                minorRightTicks.append(rightMinorTick);
            }
        }

        //Progress counters
        majorY -= majorTickSpacing;
        labelValue += 5;
    }


    //------------
}

ATCAmanDisplay::~ATCAmanDisplay()
{

}

void ATCAmanDisplay::setLineEditMeteringFixVisible(bool flag)
{
    lineEditMeteringFixVisible = flag;
}

void ATCAmanDisplay::clockUpdated()
{
    progressTimeBy(1);
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

    QGraphicsView::mousePressEvent(event);
    event->accept();
}
