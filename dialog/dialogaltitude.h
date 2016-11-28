#ifndef DIALOGALTITUDE_H
#define DIALOGALTITUDE_H

#include "atcflight.h"
#include "atcmath.h"

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class DialogAltitude;
}

class DialogAltitude : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAltitude(ATCFlight *flight, ATCSettings *settings, QWidget *parent = 0);
    ~DialogAltitude();

signals:
    void signalClosed();

private slots:
    void slotClicked(const QModelIndex &index);

private:
    Ui::DialogAltitude *ui;
    ATCFlight *flight;
    ATCSettings *settings;

    QStandardItemModel *model = nullptr;

    void dialogAltitudeSetup();
    void appendRow(QString text, QStandardItemModel *model);
};

#endif // DIALOGALTITUDE_H
