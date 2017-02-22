#ifndef DIALOGLEADERS_H
#define DIALOGLEADERS_H

#include "atcdialog.h"
#include "atcsettings.h"

namespace Ui {
class DialogLeaders;
}

class DialogLeaders : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogLeaders(ATCSettings *settings, QWidget *parent = 0);
    ~DialogLeaders();

signals:
    void signalUpdateLeaders();

private slots:
    ATC_MOUSE_HANDLER

    void on_buttonNM_clicked();
    void on_buttonMIN_clicked();
    void on_buttonApply_clicked();
    void on_buttonCancel_clicked();

private:
    Ui::DialogLeaders *uiInner;
    ATCSettings *settings;

    ATC::LeaderUnits activeUnits;

    void setActiveNM();
    void setActiveMIN();
};

#endif // DIALOGLEADERS_H
