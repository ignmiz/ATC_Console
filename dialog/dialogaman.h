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

private:
    Ui::DialogAman *uiInner;
    ATCSimulation *simulation = nullptr;
    ATCAirspace *airspace;
    ATCSettings *settings;
    QTime *time;

    QLineEdit *lineEditMeteringFix;

    void createLineEdit();
    void populateAman();
};

#endif // DIALOGAMAN_H