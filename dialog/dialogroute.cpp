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

void DialogRoute::slotClicked(const QModelIndex &index)
{

}

void DialogRoute::dialogRouteSetup()
{

}

void DialogRoute::appendRow(QString text, QStandardItemModel *model)
{
    QStandardItem *item = new QStandardItem(text);
    item->setTextAlignment(Qt::AlignCenter);
    model->appendRow(item);
}
