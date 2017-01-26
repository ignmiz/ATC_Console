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
    explicit DialogFlightCreator(ATCFlight *flight, ATCAirspace *airspace, ATCSettings *settings, ATCFlightFactory *flightFactory, ATCSimulation *simulation, QWidget *parent = 0);
    explicit DialogFlightCreator(ATCAirspace *airspace, ATCSettings *settings, ATCFlightFactory *flightFactory, ATCSimulation *simulation, QWidget *parent = 0);
    ~DialogFlightCreator();

signals:
    void signalGetLocation(QStringList fixList);
    void signalCreateFlightTag(ATCFlight *flight);
    void signalUpdateFlightTag(ATCFlight *flight);
    void signalUpdateFlightList(ATCFlight *flight);

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

    void on_tabWidget_tabBarClicked(int index);
    void on_plainTextEditRoute_textChanged();

    void on_lineEditDeparture_textChanged(const QString &arg1);
    void on_lineEditDestination_textChanged(const QString &arg1);

    void slotShowFlightCreator();
    void slotDisplayClicked(double x, double y);

    void on_comboBoxRwyDep_currentTextChanged(const QString &arg1);
    void on_comboBoxRwyDes_currentTextChanged(const QString &arg1);
    void on_comboBoxSID_currentTextChanged(const QString &arg1);
    void on_comboBoxSTAR_currentTextChanged(const QString &arg1);

private:
    Ui::DialogFlightCreator *uiInner;
    ATCFlightFactory *flightFactory;
    ATCSimulation *simulation;
    ATCAirspace *airspace;
    ATCSettings *settings;
    ATCFlight *flight = nullptr;

    QStandardItemModel *model = nullptr;

    bool routeValid = false;

    bool departureChanged = false;
    bool destinationChanged = false;

    void setRoute();
    void setCallsign();
    void setSquawk();
    void setTAS();

    bool validateRoute();

    bool verifyForm();
    void errorMessage(QString msg);

    void formSetup();
    void formSetup(ATCFlight *flight);

    void setupProcedureBoxes(ATCFlight *flight);
    void refreshFixList();
    QStringList getFixList();
};

#endif // DIALOGFLIGHTCREATOR_H
