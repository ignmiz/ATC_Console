#ifndef DIALOGROUTE_H
#define DIALOGROUTE_H

#include "atcflight.h"
#include "atcroute.h"

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class DialogRoute;
}

class DialogRoute : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRoute(ATCFlight *flight, ATCSettings *settings, QWidget *parent = 0);
    ~DialogRoute();

signals:
    void signalClosed();

private slots:
    void slotClicked(const QModelIndex &index);

private:
    Ui::DialogRoute *ui;
    ATCFlight *flight;
    ATCSettings *settings;

    QStandardItemModel *model = nullptr;

    void dialogRouteSetup();
    void appendRow(QString text, QStandardItemModel *model);
};

#endif // DIALOGROUTE_H
