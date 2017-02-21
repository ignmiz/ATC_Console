#ifndef DIALOGLEADERS_H
#define DIALOGLEADERS_H

#include "atcdialog.h"

namespace Ui {
class DialogLeaders;
}

class DialogLeaders : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogLeaders(QWidget *parent = 0);
    ~DialogLeaders();

private slots:
    ATC_MOUSE_HANDLER

private:
    Ui::DialogLeaders *uiInner;
};

#endif // DIALOGLEADERS_H
