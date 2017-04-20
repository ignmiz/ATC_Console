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

    void slotMeteringFixEntered();
    void slotHideLineEdit();
    void slotClockUpdated();

    void slotFlightLabelSelected(ATCAmanFlightLabel *label);

private:
    Ui::DialogAman *uiInner;
    ATCSimulation *simulation = nullptr;
    ATCAirspace *airspace;
    ATCSettings *settings;
    QTime *time;

    QLineEdit *lineEditMeteringFix;

    ATCAmanFlightLabel *activeLabel = nullptr;
    bool RTAgui = false;

    void createLineEdit();
    void populateAman();

    void activateRTAgui();
    void deactivateRTAgui();

    void calculateSliderPosition();
};

#endif // DIALOGAMAN_H
