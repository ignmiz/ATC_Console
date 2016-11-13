#ifndef DIALOGFLIGHTCREATOR_H
#define DIALOGFLIGHTCREATOR_H

#include "atcdialog.h"
#include "atcflightfactory.h"
#include "atcsimulation.h"

namespace Ui {
class DialogFlightCreator;
}

class DialogFlightCreator : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogFlightCreator(ATCFlightFactory *flightFactory, ATCSimulation *simulation, QWidget *parent = 0);
    ~DialogFlightCreator();

signals:
    void signalGetLocation();

private slots:
    ATC_MOUSE_HANDLER

    void on_buttonOK_clicked();
    void on_buttonCancel_clicked();
    void on_buttonSetRoute_clicked();
    void on_buttonGetLocation_clicked();
    void on_buttonFillAll_clicked();
    void on_buttonSetCallsign_clicked();
    void on_buttonSetSquawk_clicked();
    void on_buttonSetTAS_clicked();

    void on_radioButtonOwnNav_clicked();
    void on_radioButtonHDG_clicked();

    void slotShowFlightCreator();
    void slotDisplayClicked(double x, double y);

private:
    Ui::DialogFlightCreator *uiInner;
    ATCFlightFactory *flightFactory;
    ATCSimulation *simulation;

    void setRoute();
    void setCallsign();
    void setSquawk();
    void setTAS();

    bool verifyForm();
    void errorMessage(QString msg);
};

#endif // DIALOGFLIGHTCREATOR_H
