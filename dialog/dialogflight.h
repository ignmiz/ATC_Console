#ifndef DIALOGFLIGHT_H
#define DIALOGFLIGHT_H

#include "atcsimulation.h"
#include "atcdialog.h"
#include "atcflight.h"
#include "atcmath.h"
#include "atccombodelegate.h"
#include "atcflags.h"

#include <QItemSelectionModel>
#include <QStandardItemModel>
#include <QDebug>

namespace Ui {
class DialogFlight;
}

class DialogFlight : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogFlight(ATCSimulation *sim, ATCAirspace *airspace, ATC::SimCreationMode m, QWidget *parent = 0);
    ~DialogFlight();

    ATCSimulation* getSimulation();

signals:
    void signalConstructDialogFlightCreator();
    void signalConstructDialogFlightCreator(ATCFlight *flight);

    void signalConstructDialogActiveRunways(ATC::SimCreationMode m);

    void signalSimulation(ATCSimulation *sim);
    void signalActiveScenarioPath(QString path);
    void signalSetFlagSimulationValid(bool simValid);

    void signalUpdateFlightList();

private slots:
    ATC_MOUSE_HANDLER

    void slotUpdateFlightList(ATCFlight *flight);
    void slotEdit(QModelIndex index);

    void on_buttonReady_clicked();
    void on_buttonCancel_clicked();

    void on_buttonCreateFlight_clicked();
    void on_buttonEditFlight_clicked();
    void on_buttonDeleteFlight_clicked();
    void on_buttonDeleteAll_clicked();

    void on_tableViewFlights_clicked(const QModelIndex &index);
    void on_buttonActiveRunways_clicked();

    void slotAdjustUI();
    void slotAdjustUI(const QItemSelection &selected, const QItemSelection &deselected);

private:
    Ui::DialogFlight *uiInner;
    ATCSimulation *simulation;
    ATCAirspace *airspace;
    QStandardItemModel *model = nullptr;

    ATCComboDelegate *procedureDelegate = nullptr;

    ATC::SimCreationMode mode;

    int rowToEdit = -1;

    void dialogFlightSetup();
    void appendRow(ATCFlight *flight, QStandardItemModel *model);
    void modifyRow(ATCFlight *flight, int row, QStandardItemModel *model);
};

#endif // DIALOGFLIGHT_H
