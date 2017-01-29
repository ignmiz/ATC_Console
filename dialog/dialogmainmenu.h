#ifndef DIALOGMAINMENU_H
#define DIALOGMAINMENU_H

#include "atcdialog.h"
#include "atcpaths.h"
#include "atcsimulation.h"

#include <QFile>
#include <QFileDialog>

namespace Ui {
class DialogMainMenu;
}

class DialogMainMenu : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogMainMenu(QTime *startTime, QWidget *parent = 0);
    ~DialogMainMenu();

    QTime getSimStartTime();

signals:
    void signalConstructDialogFlightNew();
    void signalConstructDialogFlightEdit();
    void signalImportScenario();
    void signalExportScenario();

public slots:
    void slotSetSimStartTime(QTime time);

private slots:
    ATC_MOUSE_HANDLER

    void on_buttonNewSimulation_clicked();
    void on_buttonEditSimulation_clicked();
    void on_buttonImportSimulation_clicked();
    void on_buttonExportSimulation_clicked();

    void slotActiveScenarioPath(QString path);

private:
    Ui::DialogMainMenu *uiInner;
    QTime *startTime;

};

#endif // DIALOGMAINMENU_H
