#include "dialogleaders.h"
#include "ui_dialogleaders.h"

DialogLeaders::DialogLeaders(ATCSettings *settings, QWidget *parent) :
    ATCDialog(parent, "Leader Lines", 250, 100),
    uiInner(new Ui::DialogLeaders),
    settings(settings)
{
    uiInner->setupUi(this);
    windowSetup();

    uiInner->spinBoxLength->setValue(settings->TAG_LEADER_LENGTH);

    if(settings->TAG_LEADER_UNIT == ATC::LeaderNM)
    {
        setActiveNM();
    }
    else
    {
        setActiveMIN();
    }
}

DialogLeaders::~DialogLeaders()
{
    delete uiInner;
}

void DialogLeaders::on_buttonNM_clicked()
{
    if(activeUnits != ATC::LeaderNM) setActiveNM();
}

void DialogLeaders::on_buttonMIN_clicked()
{
    if(activeUnits != ATC::LeaderMIN) setActiveMIN();
}

void DialogLeaders::on_buttonApply_clicked()
{
    settings->TAG_LEADER_LENGTH = uiInner->spinBoxLength->value();
    settings->TAG_LEADER_UNIT = activeUnits;

    emit signalUpdateLeaders();
    emit closed();
    close();
}

void DialogLeaders::on_buttonCancel_clicked()
{
    emit closed();
    close();
}

void DialogLeaders::setActiveNM()
{
    uiInner->buttonNM->setStyleSheet(
                                    "QPushButton"
                                    "{"
                                        "color: #c8c8c8;"
                                        "font: bold 11px;"
                                        "background-color: #006400;"
                                        "border-style: outset;"
                                        "border-width: 2px;"
                                        "border-color: #3e3e3e;"
                                    "}"
                                    ""
                                    "QPushButton::hover"
                                    "{"
                                        "background-color: #007800;"
                                    "}"
                                    ""
                                    "QPushButton::pressed"
                                    "{"
                                        "background-color: #008C00;"
                                    "}"
                                );

    uiInner->buttonMIN->setStyleSheet(
                                    "QPushButton"
                                    "{"
                                        "color: #c8c8c8;"
                                        "font: 10px;"
                                        "background-color: #000000;"
                                        "border-style: outset;"
                                        "border-width: 2px;"
                                        "border-color: #3e3e3e;"
                                    "}"
                                    ""
                                    "QPushButton::hover"
                                    "{"
                                        "background-color: #2d2d2d;"
                                    "}"
                                    ""
                                    "QPushButton::pressed"
                                    "{"
                                        "background-color: #3c3c3c;"
                                    "}"
                                );

    activeUnits = ATC::LeaderNM;

    uiInner->buttonNM->update();
    uiInner->buttonMIN->update();
}

void DialogLeaders::setActiveMIN()
{
    uiInner->buttonMIN->setStyleSheet(
                                    "QPushButton"
                                    "{"
                                        "color: #c8c8c8;"
                                        "font: bold 11px;"
                                        "background-color: #006400;"
                                        "border-style: outset;"
                                        "border-width: 2px;"
                                        "border-color: #3e3e3e;"
                                    "}"
                                    ""
                                    "QPushButton::hover"
                                    "{"
                                        "background-color: #007800;"
                                    "}"
                                    ""
                                    "QPushButton::pressed"
                                    "{"
                                        "background-color: #008C00;"
                                    "}"
                                );

    uiInner->buttonNM->setStyleSheet(
                                    "QPushButton"
                                    "{"
                                        "color: #c8c8c8;"
                                        "font: 10px;"
                                        "background-color: #000000;"
                                        "border-style: outset;"
                                        "border-width: 2px;"
                                        "border-color: #3e3e3e;"
                                    "}"
                                    ""
                                    "QPushButton::hover"
                                    "{"
                                        "background-color: #2d2d2d;"
                                    "}"
                                    ""
                                    "QPushButton::pressed"
                                    "{"
                                        "background-color: #3c3c3c;"
                                    "}"
                                );

    activeUnits = ATC::LeaderMIN;

    uiInner->buttonNM->update();
    uiInner->buttonMIN->update();
}


