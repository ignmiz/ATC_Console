
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
    QGraphicsLineItem *leftVert = new QGraphicsLineItem(-30, -ATCConst::AMAN_DISPLAY_HEIGHT/2, -30, ATCConst::AMAN_DISPLAY_HEIGHT);
    leftVert->setPen(pen);
    currentScene->addItem(leftVert);

    QGraphicsLineItem *rightVert = new QGraphicsLineItem(30, -ATCConst::AMAN_DISPLAY_HEIGHT/2, 30, ATCConst::AMAN_DISPLAY_HEIGHT);
    rightVert->setPen(pen);
    currentScene->addItem(rightVert);

    //Time scale ticks spacing
    double majorTickSpacing = ATCConst::AMAN_DISPLAY_HEIGHT / 13;
    double minorTickSpacing = majorTickSpacing / 5;

    //Time scale horizon
    QGraphicsLineItem *timeHorizon = new QGraphicsLineItem(-ATCConst::AMAN_DISPLAY_WIDTH/2, ATCConst::AMAN_DISPLAY_HEIGHT/2 - majorTickSpacing, ATCConst::AMAN_DISPLAY_WIDTH/2, ATCConst::AMAN_DISPLAY_HEIGHT/2 - majorTickSpacing);
    timeHorizon->setPen(pen3);
    currentScene->addItem(timeHorizon);

    //Time scale ticks
    double innerMajorTickBound = 15;
    double outerMajorTickBound = 30;

    double innerMinorTickBound = 25;
    double outerMinorTickBound = 30;

    double majorY = -ATCConst::AMAN_DISPLAY_HEIGHT / 2;
    int labelValue = 60;

    for(int i = 0; i < 14; i++)
    {
        //Create major tick
        QGraphicsLineItem *leftMajorTick = new QGraphicsLineItem(-outerMajorTickBound,  majorY, -innerMajorTickBound, majorY);
        leftMajorTick->setPen(pen);
        currentScene->addItem(leftMajorTick);

        QGraphicsLineItem *rightMajorTick = new QGraphicsLineItem(outerMajorTickBound, majorY, innerMajorTickBound, majorY);
        rightMajorTick->setPen(pen);
        currentScene->addItem(rightMajorTick);

        //Create major tick label
        if((i > 0) && (i < 13))
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
        }

        //Create minor ticks
        if(i < 13)
        {
            for(int j = 0; j < 4; j++)
            {
                double minorY = majorY + (j + 1) * minorTickSpacing;

                QGraphicsLineItem *leftMinorTick = new QGraphicsLineItem(-outerMinorTickBound, minorY, -innerMinorTickBound, minorY);
                leftMinorTick->setPen(pen);
                currentScene->addItem(leftMinorTick);

                QGraphicsLineItem *rightMinorTick = new QGraphicsLineItem(outerMinorTickBound, minorY, innerMinorTickBound, minorY);
                rightMinorTick->setPen(pen);
                currentScene->addItem(rightMinorTick);
            }
        }

        //Progress counters
        majorY += majorTickSpacing;
        labelValue -= 5;
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
