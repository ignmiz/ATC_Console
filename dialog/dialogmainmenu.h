#ifndef DIALOGMAINMENU_H
#define DIALOGMAINMENU_H

#include "atcdialog.h"

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
    void signalConstructDialogFlightNew();
    void signalConstructDialogFlightEdit();

private slots:
    ATC_MOUSE_HANDLER

    void on_buttonNewSimulation_clicked();
    void on_buttonEditSimulation_clicked();
    void on_buttonImportSimulation_clicked();
    void on_buttonExportSimulation_clicked();

private:
    Ui::DialogMainMenu *uiInner;
};

#endif // DIALOGMAINMENU_H
