#include "dialogsettings.h"
#include "ui_dialogsettings.h"

#include <QTimer>
#include <QDebug>

DialogSettings::DialogSettings(ATCSituationalDisplay *display, QWidget *parent) :
    situationalDisplay(display),
    ATCDialog(parent, "Settings", 500, 700),
    uiInner(new Ui::DialogSettings)
{
    uiInner->setupUi(this);
    windowSetup();    
    createSettingsModel();
    setupTableView();
}

DialogSettings::~DialogSettings()
{
    delete uiInner;
    if(settingsModel != nullptr) delete settingsModel;
}

void DialogSettings::slotColorPickerClosed()
{
    flagDialogColorPickerExists = false;
}

void DialogSettings::slotUpdateColorARTCCLow(QColor color)
{
    QBrush brush(color);
    settingsModel->item(0, 1)->setBackground(brush);
    settingsModel->item(0, 1)->setForeground(brush);

    situationalDisplay->getSettings()->ARTCC_LOW_COLOR = color;
}

void DialogSettings::slotUpdateColorARTCCHigh(QColor color)
{
    QBrush brush(color);
    settingsModel->item(1, 1)->setBackground(brush);
    settingsModel->item(1, 1)->setForeground(brush);

    situationalDisplay->getSettings()->ARTCC_HIGH_COLOR = color;
}

void DialogSettings::slotUpdateColorARTCC(QColor color)
{
    QBrush brush(color);
    settingsModel->item(2, 1)->setBackground(brush);
    settingsModel->item(2, 1)->setForeground(brush);

    situationalDisplay->getSettings()->ARTCC_COLOR = color;
}

void DialogSettings::onTableClicked(const QModelIndex &index)
{
    QVariant value = index.data();

    if((value.toString() == "ARTCC Low Color") && !flagDialogColorPickerExists)
    {
//        QColor currentColor(situationalDisplay->getSettings()->ARTCC_LOW_COLOR);

//        dialogColorPicker = new DialogColorPicker(currentColor, this);
//        dialogColorPicker->show();

//        connect(dialogColorPicker, SIGNAL(signalColorPickerClosed()), this, SLOT(slotColorPickerClosed()));

//        flagDialogColorPickerExists = true;

        constructColorPicker(situationalDisplay->getSettings()->ARTCC_LOW_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorSectorARTCCLow(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateColorARTCCLow(QColor)));
    }
    else if((value.toString() == "ARTCC High Color") && !flagDialogColorPickerExists)
    {
//        QColor currentColor(situationalDisplay->getSettings()->ARTCC_HIGH_COLOR);

//        dialogColorPicker = new DialogColorPicker(currentColor, this);
//        dialogColorPicker->show();

//        connect(dialogColorPicker, SIGNAL(signalColorPickerClosed()), this, SLOT(slotColorPickerClosed()));

//        flagDialogColorPickerExists = true;

        constructColorPicker(situationalDisplay->getSettings()->ARTCC_HIGH_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorSectorARTCCHigh(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateColorARTCCHigh(QColor)));
    }
    else if((value.toString() == "ARTCC Color") && !flagDialogColorPickerExists)
    {
//        QColor currentColor(situationalDisplay->getSettings()->ARTCC_COLOR);

//        dialogColorPicker = new DialogColorPicker(currentColor, this);
//        dialogColorPicker->show();

//        connect(dialogColorPicker, SIGNAL(signalColorPickerClosed()), this, SLOT(slotColorPickerClosed()));

//        flagDialogColorPickerExists = true;

        constructColorPicker(situationalDisplay->getSettings()->ARTCC_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorSectorARTCC(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateColorARTCC(QColor)));
    }
}

void DialogSettings::setupTableView()
{
    uiInner->tableView->setModel(settingsModel);
    uiInner->tableView->setGridStyle(Qt::NoPen);

    int rowHeight = 25;
    for(int i = 0; i < settingsModel->rowCount(); i++)
    {
        uiInner->tableView->setRowHeight(i, rowHeight);
    }

    uiInner->tableView->setColumnWidth(0, 300);
    uiInner->tableView->setColumnWidth(1, 176);

    uiInner->tableView->horizontalHeader()->setHidden(true);
    uiInner->tableView->verticalHeader()->setHidden(true);

    connect(uiInner->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));
}

void DialogSettings::createSettingsModel()
{
    settingsModel = new QStandardItemModel(0, 2, this);

    settingsModel->appendRow(createSettingsRow("ARTCC Low", situationalDisplay->getSettings()->ARTCC_LOW_COLOR));
    settingsModel->appendRow(createSettingsRow("ARTCC High", situationalDisplay->getSettings()->ARTCC_HIGH_COLOR));
    settingsModel->appendRow(createSettingsRow("ARTCC", situationalDisplay->getSettings()->ARTCC_COLOR));
}

void DialogSettings::constructColorPicker(QColor &initColor)
{
    QColor currentColor(initColor);

    dialogColorPicker = new DialogColorPicker(currentColor, this);
    dialogColorPicker->show();

    connect(dialogColorPicker, SIGNAL(signalColorPickerClosed()), this, SLOT(slotColorPickerClosed()));

    flagDialogColorPickerExists = true;
}

QList<QStandardItem *> DialogSettings::createSettingsRow(QString text, QColor color)
{
    QList<QStandardItem*> items;

    QStandardItem *name = new QStandardItem(text);
    name->setFlags(Qt::NoItemFlags);
    items.append(name);

    QStandardItem *colorField = new QStandardItem(text + " Color");
    colorField->setFlags(Qt::NoItemFlags);
    items.append(colorField);

    QBrush brush(color);
    colorField->setBackground(brush);
    colorField->setForeground(brush);

    return items;
}
