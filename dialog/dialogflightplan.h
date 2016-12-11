#ifndef DIALOGFLIGHTPLAN_H
#define DIALOGFLIGHTPLAN_H

#include "atcairspace.h"
#include "atcdialog.h"
#include "atcflight.h"
#include "atcflightfactory.h"
#include "atcsimulation.h"

#include <QStandardItemModel>
#include <QDebug>

namespace Ui {
class DialogFlightPlan;
}

class DialogFlightPlan : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogFlightPlan(ATCFlight *flight, ATCAirspace *airspace, ATCSimulation *simulation, ATCFlightFactory *flightFactory, QWidget *parent = 0);
    ~DialogFlightPlan();

private slots:
    ATC_MOUSE_HANDLER

    void on_plainTextEditRoute_textChanged();
    void on_buttonFillAll_clicked();
    void on_buttonSetCallsign_clicked();
    void on_buttonSetTAS_clicked();
    void on_buttonSetSquawk_clicked();
    void on_buttonSetRoute_clicked();

    void on_buttonOK_clicked();

    void on_buttonCancel_clicked();

private:
    Ui::DialogFlightPlan *uiInner;
    ATCFlight *flight;
    ATCAirspace *airspace;
    ATCFlightFactory *flightFactory;
    ATCSimulation *simulation;

    QStandardItemModel *model = nullptr;

    bool routeValid = false;

    bool verifyForm();
    bool validateRoute();
    void errorMessage(QString msg);

    void setRoute();
    void setCallsign();
    void setSquawk();
    void setTAS();
};

#endif // DIALOGFLIGHTPLAN_H
