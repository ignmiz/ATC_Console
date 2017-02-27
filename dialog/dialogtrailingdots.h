#ifndef DIALOGTRAILINGDOTS_H
#define DIALOGTRAILINGDOTS_H

#include "atcdialog.h"
#include "atcsettings.h"

namespace Ui {
class DialogTrailingDots;
}

class DialogTrailingDots : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogTrailingDots(ATCSettings *settings, QWidget *parent = 0);
    ~DialogTrailingDots();

signals:
    void signalUpdateTrailingDots();

private slots:
    ATC_MOUSE_HANDLER

    void on_buttonApply_clicked();
    void on_buttonCancel_clicked();

private:
    Ui::DialogTrailingDots *uiInner;
    ATCSettings *settings;
};

#endif // DIALOGTRAILINGDOTS_H
