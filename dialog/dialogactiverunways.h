#ifndef DIALOGACTIVERUNWAYS_H
#define DIALOGACTIVERUNWAYS_H

#include "atcdialog.h"
#include "atcairspace.h"
#include "atcactiverunways.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QStandardItemModel>

#include <QDebug>

namespace Ui {
class DialogActiveRunways;
}

class DialogActiveRunways : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogActiveRunways(ATCAirspace *airspace, ATCActiveRunways *activeRunways, QWidget *parent = 0);
    ~DialogActiveRunways();

private slots:
    ATC_MOUSE_HANDLER

    void on_buttonOK_clicked();
    void on_buttonCancel_clicked();

    void slotCheckBoxStateChanged(int i);

private:
    Ui::DialogActiveRunways *uiInner;
    ATCAirspace *airspace;
    ATCActiveRunways *activeRunways;
    QStandardItemModel *model = nullptr;

    void dialogSetup();
    void fillModel();

    void setActive(ActiveAirport &airport);

    void appendAirport(ATCAirport *airport, QStandardItemModel *model);
    void appendRow(QString airportCode, QString rwyCode, QStandardItemModel *model);
    void appendRow(QString airportCode, QStandardItemModel *model);

    void createCenteredCheckbox(QModelIndex &index);
    QModelIndex getCheckBoxIndex(QCheckBox *checkBox);
};

#endif // DIALOGACTIVERUNWAYS_H
