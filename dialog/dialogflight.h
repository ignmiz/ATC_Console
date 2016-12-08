#ifndef DIALOGFLIGHT_H
#define DIALOGFLIGHT_H

#include "atcairspace.h"
#include "atcdialog.h"

namespace Ui {
class DialogFlight;
}

class DialogFlight : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogFlight(ATCAirspace *airspace, QWidget *parent = 0);
    ~DialogFlight();

signals:
    void signalConstructFlightCreator();

private slots:
    ATC_MOUSE_HANDLER

    void on_buttonCreateFlight_clicked();
    void on_buttonReady_clicked();
    void on_buttonCancel_clicked();

private:
    Ui::DialogFlight *uiInner;
    ATCAirspace *airspace;
};

#endif // DIALOGFLIGHT_H
