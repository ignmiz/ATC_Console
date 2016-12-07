#include "dialoghandoff.h"
#include "ui_dialoghandoff.h"

DialogHandoff::DialogHandoff(ATCFlight *flight, ATCSettings *settings, QWidget *parent) :
    flight(flight),
    settings(settings),
    QDialog(parent),
    ui(new Ui::DialogHandoff)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

    dialogHandoffSetup();

    connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotClicked(QModelIndex)));
}

DialogHandoff::~DialogHandoff()
{
    if(model != nullptr) delete model;
    delete ui;
}

void DialogHandoff::slotClicked(const QModelIndex &index)
{
    //TO BE DEVELOPED
}

void DialogHandoff::dialogHandoffSetup()
{
    model = new QStandardItemModel(this);

    //IF ASSUMED
    appendRow("TANSFER", model);
    appendRow("DROP", model);

    //IF NOT ASSUMED
    appendRow("ACCEPT", model);

    //IF DROPPED
    appendRow("ASSUME", model);

    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listView->setFocusPolicy(Qt::NoFocus);
}

void DialogHandoff::appendRow(QString text, QStandardItemModel *model)
{
    QStandardItem *item = new QStandardItem(text);
    item->setTextAlignment(Qt::AlignCenter);
    model->appendRow(item);
}
