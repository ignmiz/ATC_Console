#include "dialogaman.h"
#include "ui_dialogaman.h"

DialogAman::DialogAman(ATCAirspace *airspace, ATCSettings *settings, QTime *time, QWidget *parent) :
    ATCDialog(parent, "Approach Manager", 700, 900),
    airspace(airspace),
    settings(settings),
    time(time),
    uiInner(new Ui::DialogAman)
{
    uiInner->setupUi(this);
    windowSetup();

    uiInner->amanDisplay->setSettings(settings);
    uiInner->amanDisplay->createTimeline(time);

    deactivateRTAgui();

    createLineEdit();
    connect(uiInner->amanDisplay, SIGNAL(signalHideLineEdit()), this, SLOT(slotHideLineEdit()));
}

DialogAman::~DialogAman()
{
    if(lineEditMeteringFix != nullptr) delete lineEditMeteringFix;

    delete uiInner;
}

QPushButton *DialogAman::getClock()
{
    return uiInner->buttonClock;
}

void DialogAman::setMeteringFix(QString &fix)
{
    if(fix.isEmpty() || airspace->isValidNavaid(fix)) uiInner->buttonMeteringFix->setText(fix);
}

void DialogAman::setSimulation(ATCSimulation *sim)
{
    simulation = sim;
    populateAman();
}

void DialogAman::on_buttonMeteringFix_clicked()
{
    uiInner->buttonMeteringFix->hide();

    lineEditMeteringFix->setText(uiInner->buttonMeteringFix->text());
    lineEditMeteringFix->setStyleSheet("QLineEdit"
                                       "{"
                                       "    color: #c8c8c8;"
                                       "    background-color: #00153a;"
                                       "    border-style: inset;"
                                       "    border-width: 2px;"
                                       "    border-color: #3e3e3e;"
                                       "    font: 14px;"
                                       "}"
                                       );
    lineEditMeteringFix->selectAll();
    lineEditMeteringFix->setFocus();
    lineEditMeteringFix->show();

    uiInner->amanDisplay->setLineEditMeteringFixVisible(true);
}

void DialogAman::slotMeteringFixEntered()
{
    QString name = lineEditMeteringFix->text();

    if(!name.isEmpty() && !airspace->isValidNavaid(name))
    {
        lineEditMeteringFix->setStyleSheet("QLineEdit"
                                           "{"
                                           "    color: #ff0000;"
                                           "    background-color: #00153a;"
                                           "    border-style: inset;"
                                           "    border-width: 2px;"
                                           "    border-color: #3e3e3e;"
                                           "    font: 14px;"
                                           "}"
                                          );
        return;
    }

    lineEditMeteringFix->hide();

    uiInner->buttonMeteringFix->setText(name);
    uiInner->buttonMeteringFix->show();

    if(simulation != nullptr) simulation->setMeteringFix(name);
}

void DialogAman::slotHideLineEdit()
{
    lineEditMeteringFix->hide();
    lineEditMeteringFix->clear();


    uiInner->buttonMeteringFix->show();
}

void DialogAman::slotClockUpdated()
{
    uiInner->amanDisplay->clockUpdated();
}

void DialogAman::slotFlightLabelSelected(ATCAmanFlightLabel *label)
{
    activeLabel = label;

    if(label != nullptr)
    {
        if(!RTAgui) activateRTAgui();
    }
    else
    {
        if(RTAgui) deactivateRTAgui();
    }
}

void DialogAman::createLineEdit()
{
    lineEditMeteringFix = new QLineEdit(this);
    lineEditMeteringFix->setGeometry(290, 875, 120, 25);
    lineEditMeteringFix->setAlignment(Qt::AlignHCenter);
    lineEditMeteringFix->setInputMask(">nnnnn");
    lineEditMeteringFix->hide();

    connect(lineEditMeteringFix, SIGNAL(returnPressed()), this, SLOT(slotMeteringFixEntered()));
}

void DialogAman::populateAman()
{
    if(simulation != nullptr)
    {
        //Create labels
        ATCAmanFlightLabel *label = new ATCAmanFlightLabel(simulation->getFlight(0), QPointF(32, 100));
        label->addToScene(uiInner->amanDisplay->scene());

        uiInner->amanDisplay->appendFlightLabel(label);

        //Connect slots
        connect(label, SIGNAL(signalFlightLabelSelected(ATCAmanFlightLabel*)), uiInner->amanDisplay, SLOT(slotFlightLabelSelected(ATCAmanFlightLabel*)));
        connect(label, SIGNAL(signalFlightLabelSelected(ATCAmanFlightLabel*)), this, SLOT(slotFlightLabelSelected(ATCAmanFlightLabel*)));
        connect(label, SIGNAL(signalLabelHovered(bool)), uiInner->amanDisplay, SLOT(slotLabelHovered(bool)));
    }

    QString etiquette("CONCERNED: AAA     ON TIME: DDD\n"
                      "UNMETERED: BBB    DEVIATED: EEE\n"
                      "  METERED: CCC COMPROMISED: FFF"
                     );
    uiInner->labelStats->setText(etiquette);

    QFont font("Consolas");
    font.setPointSizeF(12);
    uiInner->labelStats->setFont(font);
}

void DialogAman::activateRTAgui()
{
    uiInner->timeEdit->setEnabled(true);
    uiInner->buttonClear->setEnabled(true);
    uiInner->horizontalSlider->setEnabled(true);
    uiInner->labelLater->setEnabled(true);
    uiInner->labelSooner->setEnabled(true);

    RTAgui = true;
}

void DialogAman::deactivateRTAgui()
{
    uiInner->timeEdit->setEnabled(false);
    uiInner->buttonClear->setEnabled(false);
    uiInner->horizontalSlider->setEnabled(false);
    uiInner->labelLater->setEnabled(false);
    uiInner->labelSooner->setEnabled(false);

    RTAgui = false;
}

