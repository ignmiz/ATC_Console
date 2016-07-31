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

    uiInner->widgetColor->setAutoFillBackground(true);

    connect(uiInner->buttonColorPicker, SIGNAL(clicked(bool)), situationalDisplay, SLOT(slotFlagARTCCLow(bool)));
}

DialogSettings::~DialogSettings()
{
    delete uiInner;

    if(settingsModel != nullptr) delete settingsModel;
}

void DialogSettings::on_buttonColorPicker_clicked()
{
    if(!flagDialogColorPickerExists)
    {
        QColor currentColor(uiInner->widgetColor->palette().color(QPalette::Window));

        dialogColorPicker = new DialogColorPicker(currentColor, this);
        dialogColorPicker->show();

        connect(dialogColorPicker, SIGNAL(signalColorPickerClosed()), this, SLOT(slotColorPickerClosed()));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotSetColor(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorSectorARTCCLow(QColor)));

        flagDialogColorPickerExists = true;
    }
}

void DialogSettings::slotColorPickerClosed()
{
    flagDialogColorPickerExists = false;
    disconnect(dialogColorPicker, SIGNAL(signalColorPickerClosed()), this, SLOT(slotColorPickerClosed()));
    disconnect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotSetColor(QColor)));
    disconnect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorSectorARTCCLow(QColor)));
}

void DialogSettings::slotSetColor(QColor color)
{
    QPalette palette = uiInner->widgetColor->palette();
    palette.setColor(QPalette::Window, color);

    uiInner->widgetColor->setPalette(palette);
}

void DialogSettings::createSettingsModel()
{
    settingsModel = new QStandardItemModel(0, 2, this);

    QList<QStandardItem*> items;

    QStandardItem *name = new QStandardItem("ARTCC Low");
    name->setFlags(Qt::NoItemFlags);
    items.append(name);

    QStandardItem *colorField = new QStandardItem("<Color here>");
    colorField->setFlags(Qt::NoItemFlags);
    items.append(colorField);

    settingsModel->appendRow(items);

    uiInner->tableView->setModel(settingsModel);
//    uiInner->tableView->setGridStyle(Qt::NoPen);
    uiInner->tableView->setColumnWidth(0, 300);
    uiInner->tableView->setColumnWidth(1, 178);
    uiInner->tableView->horizontalHeader()->setHidden(true);
    uiInner->tableView->verticalHeader()->setHidden(true);
}

//void DialogSettings::initializeColorBoxes()
//{
//    QColor color(situationalDisplay->getSettings()->ARTCC_LOW_COLOR);

//    QPalette palette;
//    palette.setColor(QPalette::Window, color);

//    uiInner->widgetColor->setPalette(palette);
//}
