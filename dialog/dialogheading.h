#ifndef DIALOGHEADING_H
#define DIALOGHEADING_H

#include "atcflight.h"

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class DialogHeading;
}

class DialogHeading : public QDialog
{
    Q_OBJECT

public:
    explicit DialogHeading(ATCFlight *flight, ATCSettings *settings, QWidget *parent = 0);
    ~DialogHeading();

signals:
    void signalClosed();
    void signalUpdateRoute(ATCFlight *flight);

private slots:
    void on_buttonILS_clicked();
    void slotClicked(const QModelIndex &index);

private:
    Ui::DialogHeading *ui;
    ATCFlight *flight;
    ATCSettings *settings;

    QStandardItemModel *model = nullptr;

    void dialogHeadingSetup();
    void appendRow(QString text, QStandardItemModel *model);
};

#endif // DIALOGHEADING_H
