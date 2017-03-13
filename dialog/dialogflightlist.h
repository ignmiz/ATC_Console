#ifndef DIALOGFLIGHTLIST_H
#define DIALOGFLIGHTLIST_H

#include "atcdialog.h"

namespace Ui {
class DialogFlightList;
}

class DialogFlightList : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogFlightList(QWidget *parent = 0);
    ~DialogFlightList();

private slots:
    ATC_MOUSE_HANDLER

private:
    Ui::DialogFlightList *uiInner;
};

#endif // DIALOGFLIGHTLIST_H
