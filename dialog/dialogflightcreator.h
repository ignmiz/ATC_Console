#ifndef DIALOGFLIGHTCREATOR_H
#define DIALOGFLIGHTCREATOR_H

#include "atcdialog.h"
#include "atcflightfactory.h"

namespace Ui {
class DialogFlightCreator;
}

class DialogFlightCreator : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogFlightCreator(ATCFlightFactory *flightFactory, QWidget *parent = 0);
    ~DialogFlightCreator();

private slots:
    ATC_MOUSE_HANDLER

    void on_buttonOK_clicked();
    void on_buttonCancel_clicked();
    void on_buttonGetRoute_clicked();
    void on_buttonGetLocation_clicked();
    void on_buttonAutoFill_clicked();

private:
    Ui::DialogFlightCreator *uiInner;
    ATCFlightFactory *flightFactory;
};

#endif // DIALOGFLIGHTCREATOR_H
