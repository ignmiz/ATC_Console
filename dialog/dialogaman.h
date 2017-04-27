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

    void countRTAs();

private slots:
    ATC_MOUSE_HANDLER

    void on_buttonMeteringFix_clicked();
    void on_buttonSetRTA_clicked();
    void on_buttonClear_clicked();
    void on_buttonClearAll_clicked();

    void slotMeteringFixEntered();
    void slotHideLineEdit();
    void slotClockUpdated();

    void slotFlightLabelSelected(ATCAmanFlightLabel *label);
    void slotValueChanged(int value);
    void slotTimeChanged(QTime t);
    void slotSliderPressed();
    void slotSliderReleased();

    void slotMeteringFixFound(ATCFlight *flight);
    void slotMeteringFixLost(ATCFlight *flight);

    void slotUpdateStatistics();
    void slotScrollBy(double i);

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

    int pageNumber;
    double pageDelta;

    int RTAcount = 0;
    int onTimeCount = 0;
    int deviatedCount = 0;
    int compromisedCount = 0;

    QHash<ATCFlight*, ATCAmanFlightLabel*> labelsHash;

    void createLineEdit();
    void updateStatsEtiquette();

    void createSelector();
    void createTimeRangeBar();

    void activateRTAgui(bool hasRTA);
    void deactivateRTAgui();

    void toggleClearAll();

    QTime timeFromY(double y);
    double timeToY(QTime &t);

    void initializeSliderPosition();
    void initializeTimeEditValue();

    void updateLabelPosition(ATCAmanFlightLabel *label);
    bool isWithinSceneBoundaries(ATCAmanFlightLabel *label);
};

#endif // DIALOGAMAN_H
