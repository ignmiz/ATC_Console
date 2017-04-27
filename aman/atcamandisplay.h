#ifndef ATCAMANDISPLAY_H
#define ATCAMANDISPLAY_H

#include "atcconst.h"
#include "atcsettings.h"
#include "atcamanflightlabel.h"

#include <QTime>
#include <QSet>
#include <QPushButton>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QDebug>

class ATCAmanDisplay : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ATCAmanDisplay(QWidget *parent = 0);
    ~ATCAmanDisplay();

    void setSettings(ATCSettings *s);
    void createTimeline(QTime *t);

    void insertFlightLabel(ATCAmanFlightLabel *lbl);
    QSet<ATCAmanFlightLabel*>& getFlightLabels();
    void removeFlightLabel(ATCAmanFlightLabel *lbl);

    void setLineEditMeteringFixVisible(bool flag);
    void clockUpdated();

    double getPageDelta();
    int getPageNumber();

signals:
    void signalHideLineEdit();
    void signalScrollBy(int i);

private slots:
    void slotFlightLabelSelected(ATCAmanFlightLabel *label);
    void slotLabelHovered(bool flag);

private:
    ATCSettings *settings;

    QGraphicsScene *currentScene;
    QTime *time;

    QGraphicsLineItem *timeHorizon;
    QGraphicsLineItem *leftBar;
    QGraphicsLineItem *rightBar;
    QList<QGraphicsLineItem*> minorLeftTicks;
    QList<QGraphicsLineItem*> minorRightTicks;
    QList<QGraphicsLineItem*> majorLeftTicks;
    QList<QGraphicsLineItem*> majorRightTicks;
    QList<QGraphicsSimpleTextItem*> labels;

    QSet<ATCAmanFlightLabel*> flightLabels;
    ATCAmanFlightLabel *activeLabel = nullptr;

    double majorTickSpacing;
    double minorTickSpacing;

    int currentHour;
    int overflowCounter;
    int lowestLabel;

    bool lineEditMeteringFixVisible;
    bool timelineCreated = false;

    bool labelHovered = false;

    double pageDelta = 5;
    int pageNumber = 0;

    void initializeTimelinePosition();
    void progressTimeBy(double seconds);

protected:
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

#endif // ATCAMANDISPLAY_H
