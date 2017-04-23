#ifndef DIALOGAMAN_H
#define DIALOGAMAN_H

#include "atcdialog.h"
#include "atcairspace.h"
#include "atcsettings.h"
#include "atcsimulation.h"
#include "atcamanflightlabel.h"

#include <QLineEdit>
#include <QList>
#include <QDebug>

namespace Ui {
class DialogAman;
}

class DialogAman : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogAman(ATCAirspace *airspace, ATCSettings *settings, QTime *time, QWidget *parent = 0);
    ~DialogAman();

    QPushButton* getClock();

    void setMeteringFix(QString &fix);
    void setSimulation(ATCSimulation *sim);

private slots:
    ATC_MOUSE_HANDLER

    void on_buttonMeteringFix_clicked();
    void on_buttonSetRTA_clicked();

    void slotMeteringFixEntered();
    void slotHideLineEdit();
    void slotClockUpdated();

    void slotFlightLabelSelected(ATCAmanFlightLabel *label);
    void slotValueChanged(int value);
    void slotTimeChanged(QTime t);
    void slotSliderPressed();
    void slotSliderReleased();

private:
    Ui::DialogAman *uiInner;
    ATCSimulation *simulation = nullptr;
    ATCAirspace *airspace;
    ATCSettings *settings;
    QTime *time;

    QLineEdit *lineEditMeteringFix;
    ATCAmanFlightLabel *activeLabel = nullptr;

    bool RTAgui = false;
    bool flagSliderPressed = false;

    int pageNumber = 0;
    double pageDelta = 1200;   //seconds

    void createLineEdit();
    void populateAman();

    void createSelector();
    void createTimeRangeBar();

    void activateRTAgui();
    void deactivateRTAgui();

    QTime timeFromY(double y);
    double timeToY(QTime &t);

    void initializeSliderPosition();
    void initializeTimeEditValue();
};

#endif // DIALOGAMAN_H
