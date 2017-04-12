#ifndef DIALOGAMAN_H
#define DIALOGAMAN_H

#include "atcdialog.h"

namespace Ui {
class DialogAman;
}

class DialogAman : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogAman(QWidget *parent = 0);
    ~DialogAman();

private slots:
    ATC_MOUSE_HANDLER

private:
    Ui::DialogAman *uiInner;
};

#endif // DIALOGAMAN_H
