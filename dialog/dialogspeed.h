#ifndef DIALOGSPEED_H
#define DIALOGSPEED_H

#include "atcflight.h"
#include "atcmath.h"

#include <QDialog>
#include <QStandardItemModel>
#include <QDebug>

namespace Ui {
class DialogSpeed;
}

class DialogSpeed : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSpeed(ATCFlight *flight, ATCSettings *settings, QWidget *parent = 0);
    ~DialogSpeed();

signals:
    void signalClosed();

private slots:
    void slotClicked(const QModelIndex &index);

private:
    Ui::DialogSpeed *ui;
    ATCFlight *flight;
    ATCSettings *settings;

    QStandardItemModel *model = nullptr;

    void dialogSpeedSetup();
    void appendRow(QString text, QStandardItemModel *model);
};

#endif // DIALOGSPEED_H
