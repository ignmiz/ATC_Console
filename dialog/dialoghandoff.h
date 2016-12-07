#ifndef DIALOGHANDOFF_H
#define DIALOGHANDOFF_H

#include "atcflight.h"

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class DialogHandoff;
}

class DialogHandoff : public QDialog
{
    Q_OBJECT

public:
    explicit DialogHandoff(ATCFlight *flight, ATCSettings *settings, QWidget *parent = 0);
    ~DialogHandoff();

signals:
    void signalClosed();

private slots:
    void slotClicked(const QModelIndex &index);

private:
    Ui::DialogHandoff *ui;
    ATCFlight *flight;
    ATCSettings *settings;

    QStandardItemModel *model = nullptr;

    void dialogHandoffSetup();
    void appendRow(QString text, QStandardItemModel *model);
};

#endif // DIALOGHANDOFF_H
