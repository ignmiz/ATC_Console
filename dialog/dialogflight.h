#ifndef DIALOGFLIGHT_H
#define DIALOGFLIGHT_H

#include "atcdialog.h"

namespace Ui {
class DialogFlight;
}

class DialogFlight : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogFlight(QWidget *parent = 0);
    ~DialogFlight();

private slots:
    ATC_MOUSE_HANDLER

    void on_buttonCreateEmpty_clicked();
    void on_buttonCreateFlight_clicked();
    void on_buttonReady_clicked();
    void on_buttonCancel_clicked();

private:
    Ui::DialogFlight *uiInner;
};

#endif // DIALOGFLIGHT_H
