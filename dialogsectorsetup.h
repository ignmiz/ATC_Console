#ifndef DIALOGSECTORSETUP_H
#define DIALOGSECTORSETUP_H

#include "atcdialog.h"

namespace Ui {
class DialogSectorSetup;
}

class DialogSectorSetup : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogSectorSetup(QWidget *parent = 0);
    ~DialogSectorSetup();

private slots:
    ATC_MOUSE_HANDLER

private:
    Ui::DialogSectorSetup *uiInner;
};

#endif // DIALOGSECTORSETUP_H
