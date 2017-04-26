#ifndef ATCAMANDISPLAY_H
#define ATCAMANDISPLAY_H

#include "atcconst.h"
#include "atcsettings.h"
#include "atcamanflightlabel.h"

#include <QTime>
#include <QSet>
#include <QPushButton>
#include <QMouseEvent>
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

signals:
    void signalHideLineEdit();

private slots:
    void slotFlightLabelSelected(ATCAmanFlightLabel *label);
    void slotLabelHovered(bool flag);

private:
    ATCSettings *settings;

    QGraphicsScene *currentScene;
    QTime *time;

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

    void initializeTimelinePosition();
    void progressTimeBy(double seconds);

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // ATCAMANDISPLAY_H
