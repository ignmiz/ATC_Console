#ifndef DIALOGACTIVERUNWAYS_H
#define DIALOGACTIVERUNWAYS_H

#include "atcdialog.h"

namespace Ui {
class DialogActiveRunways;
}

class DialogActiveRunways : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogActiveRunways(QWidget *parent = 0);
    ~DialogActiveRunways();

private slots:
    ATC_MOUSE_HANDLER

    void on_buttonOK_clicked();
    void on_buttonCancel_clicked();

private:
    Ui::DialogActiveRunways *uiInner;
};

#endif // DIALOGACTIVERUNWAYS_H
