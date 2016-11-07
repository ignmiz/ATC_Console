#ifndef DIALOGMAINMENU_H
#define DIALOGMAINMENU_H

#include "atcdialog.h"
#include "dialogflight.h"

namespace Ui {
class DialogMainMenu;
}

class DialogMainMenu : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogMainMenu(QWidget *parent = 0);
    ~DialogMainMenu();

signals:
    void signalConstructDialogFlight();

private slots:
    ATC_MOUSE_HANDLER

    void on_buttonTest_clicked();

private:
    Ui::DialogMainMenu *uiInner;
};

#endif // DIALOGMAINMENU_H
