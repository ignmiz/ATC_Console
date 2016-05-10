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

private slots:
    ATC_MOUSE_HANDLER

private:
    Ui::DialogMainMenu *uiInner;
};

#endif // DIALOGMAINMENU_H
