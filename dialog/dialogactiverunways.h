#ifndef DIALOGACTIVERUNWAYS_H
#define DIALOGACTIVERUNWAYS_H

#include "atcdialog.h"
#include "atcairspace.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QStandardItemModel>

namespace Ui {
class DialogActiveRunways;
}

class DialogActiveRunways : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogActiveRunways(ATCAirspace *airspace, QWidget *parent = 0);
    ~DialogActiveRunways();

private slots:
    ATC_MOUSE_HANDLER

    void on_buttonOK_clicked();
    void on_buttonCancel_clicked();

private:
    Ui::DialogActiveRunways *uiInner;
    ATCAirspace *airspace;
    QStandardItemModel *model = nullptr;

    void dialogSetup();

    void appendAirport(ATCAirport *airport, QStandardItemModel *model);
    void appendRow(QString airportCode, QString rwyCode, QStandardItemModel *model);
    void appendRow(QString airportCode, QStandardItemModel *model);

    void createCenteredCheckbox(QModelIndex &index);
};

#endif // DIALOGACTIVERUNWAYS_H
