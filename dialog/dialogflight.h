#ifndef DIALOGFLIGHT_H
#define DIALOGFLIGHT_H

#include "atcsimulation.h"
#include "atcdialog.h"
#include "atcflight.h"
#include "atcmath.h"
#include "atccombodelegate.h"

#include <QStandardItemModel>
#include <QDebug>

namespace Ui {
class DialogFlight;
}

class DialogFlight : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogFlight(ATCSimulation *simulation, ATCAirspace *airspace, QWidget *parent = 0);
    ~DialogFlight();

signals:
    void signalConstructDialogFlightCreator();
    void signalConstructDialogFlightCreator(ATCFlight *flight);

    void signalConstructDialogActiveRunways();

private slots:
    ATC_MOUSE_HANDLER

    void slotUpdateFlightList(ATCFlight *flight);

    void on_buttonReady_clicked();
    void on_buttonCancel_clicked();

    void on_buttonCreateFlight_clicked();
    void on_buttonEditFlight_clicked();
    void on_buttonDeleteFlight_clicked();
    void on_buttonDeleteAll_clicked();

    void on_buttonActiveRunways_clicked();

    void slotEdit(QModelIndex index);

private:
    Ui::DialogFlight *uiInner;
    ATCSimulation *simulation;
    ATCAirspace *airspace;
    QStandardItemModel *model = nullptr;

    ATCComboDelegate *procedureDelegate = nullptr;

    int rowToEdit = -1;

    void dialogFlightSetup();
    void appendRow(ATCFlight *flight, QStandardItemModel *model);
    void modifyRow(ATCFlight *flight, int row, QStandardItemModel *model);
};

#endif // DIALOGFLIGHT_H
