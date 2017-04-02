#include "dialogaltitude.h"
#include "ui_dialogaltitude.h"

DialogAltitude::DialogAltitude(ATCFlight *flight, ATCSettings *settings, QWidget *parent) :
    flight(flight),
    settings(settings),
    QDialog(parent),
    ui(new Ui::DialogAltitude)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

    dialogAltitudeSetup();

    connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotClicked(QModelIndex)));
}

DialogAltitude::~DialogAltitude()
{
    if(model != nullptr) delete model;
    delete ui;
}

void DialogAltitude::slotClicked(const QModelIndex &index)
{
    if(!flight->isGlidePath())
    {
        double altitude = ATCMath::m2ft(flight->getState().h) / 100;
        QString altitudeString = index.data(Qt::DisplayRole).toString();

        if(altitude >= settings->TRANSITION_LEVEL)
        {
            flight->setTargetAltitude("F" + altitudeString);
        }
        else
        {
            flight->setTargetAltitude("A" + altitudeString);
        }

        QString shortEtiquette = flight->getFlightTag()->getTagBox()->getShortEtiquette();
        QString longEtiquette = flight->getFlightTag()->getTagBox()->getLongEtiquette();

        for(int i = 0; i < altitudeString.size(); i++)
        {
            shortEtiquette[i + 20] = altitudeString.at(i);
            longEtiquette[i + 20] = altitudeString.at(i);
        }

        flight->getFlightTag()->getTagBox()->setShortEtiquette(shortEtiquette);
        flight->getFlightTag()->getTagBox()->setLongEtiquette(longEtiquette);

        if(flight->getFlightTag()->getTagType() == ATC::Short)
        {
            flight->getFlightTag()->getTagBox()->setShort();
        }
        else
        {
            flight->getFlightTag()->getTagBox()->setLong();
        }
    }

    emit signalUpdateFlightList(flight);
    emit signalClosed();
    close();
}

void DialogAltitude::dialogAltitudeSetup()
{
    model = new QStandardItemModel(this);

    for(int i = 590; i >= 410; i -= 20)
    {
        appendRow(QString::number(i), model);
    }

    for(int i = 400; i >= settings->TRANSITION_LEVEL; i-=10)
    {
        if(i < 100)
        {
            appendRow("0" + QString::number(i), model);
        }
        else
        {
            appendRow(QString::number(i), model);
        }
    }

    if(settings->TRANSITION_LEVEL % 10 == 0)
    {
        for(int i = settings->TRANSITION_LEVEL - 5; i >= 10; i -= 5)
        {
            if(i < 100)
            {
                appendRow("0" + QString::number(i), model);
            }
            else
            {
                appendRow(QString::number(i), model);
            }
        }
    }
    else
    {
        for(int i = settings->TRANSITION_LEVEL; i >= 10; i -= 5)
        {
            if(i < 100)
            {
                appendRow("0" + QString::number(i), model);
            }
            else
            {
                appendRow(QString::number(i), model);
            }
        }
    }

    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listView->setFocusPolicy(Qt::NoFocus);

    QString altitudeString = flight->getTargetAltitude().right(3);

    for(int i = 0; i < model->rowCount(); i++)
    {
        if(model->index(i, 0).data(Qt::DisplayRole).toString() == altitudeString)
        {
            ui->listView->scrollTo(model->index(i, 0), QAbstractItemView::PositionAtCenter);
        }
    }

    if(flight->isCldFinalApp())
    {
        ui->buttonILS->setStyleSheet(
                                        "QPushButton"
                                        "{"
                                            "color: #c8c8c8;"
                                            "font: bold 11px;"
                                            "background-color: #574A00;"
                                            "border-style: outset;"
                                            "border-width: 2px;"
                                            "border-color: #3e3e3e;"
                                        "}"
                                        ""
                                        "QPushButton::hover"
                                        "{"
                                            "background-color: #705F00;"
                                        "}"
                                        ""
                                        "QPushButton::pressed"
                                        "{"
                                            "background-color: #8A7500;"
                                        "}"
                                    );
        ui->buttonILS->update();
    }
    else if(flight->isFinalApp())
    {
        ui->buttonILS->setStyleSheet(
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
        ui->buttonILS->update();
    }

    if(flight->getRunwayDestination().isEmpty()) ui->buttonILS->setDisabled(true);
}

void DialogAltitude::appendRow(QString text, QStandardItemModel *model)
{
    QStandardItem *item = new QStandardItem(text);
    item->setTextAlignment(Qt::AlignCenter);
    model->appendRow(item);
}

void DialogAltitude::on_buttonILS_clicked()
{
    if(flight->isFinalApp() || flight->isCldFinalApp())
    {
        flight->setCldFinalApp(false);
        flight->setFinalApp(false);
        flight->setGlidePath(false);

        ui->buttonILS->setStyleSheet(
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
        ui->buttonILS->update();
    }
    else
    {
        if(!flight->getRunwayDestination().isEmpty())
        {
            flight->setCldFinalApp(true);
            ui->buttonILS->setStyleSheet(
                                            "QPushButton"
                                            "{"
                                                "color: #c8c8c8;"
                                                "font: bold 11px;"
                                                "background-color: #574A00;"
                                                "border-style: outset;"
                                                "border-width: 2px;"
                                                "border-color: #3e3e3e;"
                                            "}"
                                            ""
                                            "QPushButton::hover"
                                            "{"
                                                "background-color: #705F00;"
                                            "}"
                                            ""
                                            "QPushButton::pressed"
                                            "{"
                                                "background-color: #8A7500;"
                                            "}"
                                        );
            ui->buttonILS->update();
        }
    }
}
