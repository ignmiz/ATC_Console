#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include "atcdialog.h"

namespace Ui {
class DialogSettings;
}

class DialogSettings : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(QWidget *parent = 0);
    ~DialogSettings();

private slots:
    ATC_MOUSE_HANDLER

private:
    Ui::DialogSettings *uiInner;
};

#endif // DIALOGSETTINGS_H
