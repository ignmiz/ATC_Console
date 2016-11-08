#ifndef DIALOGFLIGHTCREATOR_H
#define DIALOGFLIGHTCREATOR_H

#include "atcdialog.h"

namespace Ui {
class DialogFlightCreator;
}

class DialogFlightCreator : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogFlightCreator(QWidget *parent = 0);
    ~DialogFlightCreator();

private slots:
    ATC_MOUSE_HANDLER



private:
    Ui::DialogFlightCreator *uiInner;
};

#endif // DIALOGFLIGHTCREATOR_H
