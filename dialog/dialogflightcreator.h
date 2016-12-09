#ifndef DIALOGFLIGHTCREATOR_H
#define DIALOGFLIGHTCREATOR_H

#include "atcairspace.h"
#include "atcdialog.h"
#include "atcflightfactory.h"
#include "atcsimulation.h"

#include <QDebug>
#include <QStandardItemModel>

namespace Ui {
class DialogFlightCreator;
}

class DialogFlightCreator : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogFlightCreator(ATCAirspace *airspace, ATCFlightFactory *flightFactory, ATCSimulation *simulation, QWidget *parent = 0);
    ~DialogFlightCreator();

signals:
    void signalGetLocation();
    void signalCreateFlightTag(ATCFlight *flight);

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

    void on_tabWidget_tabBarClicked(int index);
    void on_plainTextEditRoute_textChanged();

private:
    Ui::DialogFlightCreator *uiInner;
    ATCFlightFactory *flightFactory;
    ATCSimulation *simulation;
    ATCAirspace *airspace;

    QStandardItemModel *model = nullptr;

    bool routeValid = false;

    void setRoute();
    void setCallsign();
    void setSquawk();
    void setTAS();

    bool validateRoute();

    bool verifyForm();
    void errorMessage(QString msg);
};

#endif // DIALOGFLIGHTCREATOR_H
