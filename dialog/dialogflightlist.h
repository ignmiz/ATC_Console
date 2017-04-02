#ifndef DIALOGFLIGHTLIST_H
#define DIALOGFLIGHTLIST_H

#include "atcdialog.h"
#include "atccombodelegate.h"

#include <QStandardItemModel>

namespace Ui {
class DialogFlightList;
}

class DialogFlightList : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogFlightList(ATCAirspace *airspace, ATCSimulation *simulation, QWidget *parent = 0);
    ~DialogFlightList();

    void setSimulation(ATCSimulation *sim);

    void clearFlightList();
    void fillFlightList();

signals:
    void signalCreateDialogFlightPlan(ATCFlight *flight);

public slots:
    void slotUpdateFlightList();
    void slotUpdateFlightList(ATCFlight *flight);

private slots:
    ATC_MOUSE_HANDLER

    void on_tableViewList_clicked(const QModelIndex &index);
    void slotEdit(QModelIndex index);

private:
    Ui::DialogFlightList *uiInner;
    ATCAirspace *airspace;
    ATCSimulation *simulation;

    QStandardItemModel *model;
    ATCComboDelegate *procedureDelegate = nullptr;

    void dialogFlightListSetup();
    void formatList();
    void createComboDelegate();
    void initializeComboDelegate();

    void appendRow(ATCFlight *flight, QStandardItemModel *model);
};

#endif // DIALOGFLIGHTLIST_H
