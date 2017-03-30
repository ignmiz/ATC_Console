#include "dialogroute.h"
#include "ui_dialogroute.h"

DialogRoute::DialogRoute(ATCFlight *flight, ATCSettings *settings, QWidget *parent) :
    flight(flight),
    settings(settings),
    QDialog(parent),
    ui(new Ui::DialogRoute)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

    dialogRouteSetup();

    connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotClicked(QModelIndex)));
}

DialogRoute::~DialogRoute()
{
    if(model != nullptr) delete model;
    delete ui;
}

void DialogRoute::on_buttonILS_clicked()
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

void DialogRoute::slotClicked(const QModelIndex &index)
{
    QString nextFix = index.data(Qt::DisplayRole).toString();

    flight->setNextFix(nextFix);
    flight->setWaypointIndex(index.row());
    flight->setDCT(true);

    QString shortEtiquette = flight->getFlightTag()->getTagBox()->getShortEtiquette();
    QString longEtiquette = flight->getFlightTag()->getTagBox()->getLongEtiquette();

    for(int i = nextFix.length(); i < 5; i++)
    {
        nextFix.append(' ');
    }

    for(int i = 0; i < nextFix.size(); i++)
    {
        shortEtiquette[i + 24] = nextFix.at(i);
        longEtiquette[i + 24] = nextFix.at(i);
    }

    flight->getFlightTag()->getTagBox()->setShortEtiquette(shortEtiquette);
    flight->getFlightTag()->getTagBox()->setLongEtiquette(longEtiquette);

    if(flight->getNavMode() == ATC::Hdg)
    {
        for(int i = 0; i < 3; i++)
        {
            longEtiquette[i + 39] = '-';
        }

        flight->getFlightTag()->getTagBox()->setLongEtiquette(longEtiquette);
        flight->setNavMode(ATC::Nav);

        if((flight->getTargetSpeed() == "---") || flight->getTargetSpeed().isEmpty())
        {
            flight->getFlightTag()->getTagBox()->rectLong2Short();
            flight->getFlightTag()->getTagBox()->setShort();
            flight->getFlightTag()->setTagType(ATC::Short);
        }
        else
        {
            flight->getFlightTag()->getTagBox()->setLong();
        }
    }
    else
    {
        if(flight->getFlightTag()->getTagType() == ATC::Short)
        {
            flight->getFlightTag()->getTagBox()->setShort();
        }
        else
        {
            flight->getFlightTag()->getTagBox()->setLong();
        }
    }

    if(flight->isFinalApp())
    {
        flight->setCldFinalApp(false);
        flight->setFinalApp(false);
        flight->setGlidePath(false);
    }

    flight->setAccuratePredictionFlag(false);
    if(flight->getRoutePrediction() != nullptr) emit signalUpdateRoute(flight);

    emit signalClosed();
    close();
}

void DialogRoute::dialogRouteSetup()
{
    model = new QStandardItemModel(this);
    QStringList routeList = flight->getFixList();

    for(int i = 0; i < routeList.size(); i++)
    {
        appendRow(routeList.at(i), model);
    }

    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listView->setFocusPolicy(Qt::NoFocus);

    QString nextFix = flight->getNextFix();

    for(int i = 0; i < model->rowCount(); i++)
    {
        if(model->index(i, 0).data(Qt::DisplayRole).toString() == nextFix)
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

void DialogRoute::appendRow(QString text, QStandardItemModel *model)
{
    QStandardItem *item = new QStandardItem(text);
    item->setTextAlignment(Qt::AlignCenter);
    model->appendRow(item);
}
