#ifndef DIALOGFLIGHTPLAN_H
#define DIALOGFLIGHTPLAN_H

#include "atcdialog.h"
#include "atcflight.h"

namespace Ui {
class DialogFlightPlan;
}

class DialogFlightPlan : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogFlightPlan(ATCFlight *flight, QWidget *parent = 0);
    ~DialogFlightPlan();

private slots:
    ATC_MOUSE_HANDLER

private:
    Ui::DialogFlightPlan *uiInner;
    ATCFlight *flight;
};

#endif // DIALOGFLIGHTPLAN_H
