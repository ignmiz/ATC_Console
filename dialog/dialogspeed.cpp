#include "dialogspeed.h"
#include "ui_dialogspeed.h"

DialogSpeed::DialogSpeed(ATCFlight *flight, ATCSettings *settings, QWidget *parent) :
    flight(flight),
    settings(settings),
    QDialog(parent),
    ui(new Ui::DialogSpeed)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

    dialogSpeedSetup();

    connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotClicked(QModelIndex)));
}

DialogSpeed::~DialogSpeed()
{
    if(model != nullptr) delete model;
    delete ui;
}

void DialogSpeed::slotClicked(const QModelIndex &index)
{
    QString speedString = index.data(Qt::DisplayRole).toString();

    flight->setTargetSpeed(speedString);

    QString shortEtiquette = flight->getFlightTag()->getTagBox()->getShortEtiquette();
    QString longEtiquette = flight->getFlightTag()->getTagBox()->getLongEtiquette();

    if((speedString.at(1) == '.') && (speedString.toDouble() >= 1))
    {
        for(int i = 0; i < speedString.size(); i++)
        {
            longEtiquette[i + 43] = speedString.at(i);
        }
    }
    else if(speedString.at(1) == '.')
    {
        speedString = "M" + speedString.right(3);

        for(int i = 0; i < speedString.size(); i++)
        {
            longEtiquette[i + 43] = speedString.at(i);
        }
    }
    else
    {
        speedString = "S" + speedString;

        for(int i = 0; i < speedString.size(); i++)
        {
            longEtiquette[i + 43] = speedString.at(i);
        }
    }

    flight->getFlightTag()->getTagBox()->setShortEtiquette(shortEtiquette);
    flight->getFlightTag()->getTagBox()->setLongEtiquette(longEtiquette);

    if(flight->getFlightTag()->getTagType() == ATC::Short)
    {
        if(speedString == "S---")
        {
            flight->getFlightTag()->getTagBox()->setShort();
        }
        else
        {
            flight->getFlightTag()->getTagBox()->rectShort2Long();
            flight->getFlightTag()->setTagType(ATC::Full);
            flight->getFlightTag()->getTagBox()->setLong();
        }
    }
    else
    {
        if((speedString == "S---") && (flight->getNavMode() == ATC::Nav))
        {
            flight->getFlightTag()->getTagBox()->rectLong2Short();
            flight->getFlightTag()->setTagType(ATC::Short);
            flight->getFlightTag()->getTagBox()->setShort();
        }
        else
        {
            flight->getFlightTag()->getTagBox()->setLong();
        }
    }

    emit signalClosed();
    close();
}

void DialogSpeed::dialogSpeedSetup()
{
    model = new QStandardItemModel(this);

    double crossover = ATCMath::crossoverAltitude(ATCMath::kt2mps(flight->getFlightPlan()->getType()->getVelocity().V_CL2_AV), flight->getFlightPlan()->getType()->getVelocity().M_CL_AV);
    double altitude = flight->getState().h;

    if(altitude <= crossover)
    {
        for(int i = 400; i >= 60; i -= 10)
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
        for(int i = 200; i >= 30; i--)
        {
            if((i % 100 != 0) && (i % 10 !=0))
            {
                appendRow(QString::number(static_cast<double>(i) / 100), model);
            }
            else if(i % 100 == 0)
            {
                appendRow(QString::number(i / 100).append(".00"), model);
            }
            else if(i % 10 == 0)
            {
                appendRow(QString::number(static_cast<double>(i) / 100).append('0'), model);
            }
        }

    }

    appendRow("---", model);

    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listView->setFocusPolicy(Qt::NoFocus);

    QString speedString = flight->getTargetSpeed();

    for(int i = 0; i < model->rowCount(); i++)
    {
        if(model->index(i, 0).data(Qt::DisplayRole).toString() == speedString)
        {
            ui->listView->scrollTo(model->index(i, 0), QAbstractItemView::PositionAtCenter);
        }
    }
}

void DialogSpeed::appendRow(QString text, QStandardItemModel *model)
{
    QStandardItem *item = new QStandardItem(text);
    item->setTextAlignment(Qt::AlignCenter);
    model->appendRow(item);
}
