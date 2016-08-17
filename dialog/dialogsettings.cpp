#include "dialogsettings.h"
#include "ui_dialogsettings.h"

#include <QMessageBox>
#include <QDebug>

DialogSettings::DialogSettings(ATCSituationalDisplay *display, QWidget *parent) :
    situationalDisplay(display),
    ATCDialog(parent, "Settings", 500, 700),
    uiInner(new Ui::DialogSettings)
{
    uiInner->setupUi(this);
    windowSetup();

    createModelSymbology();
    createModelDisplay();

    setupViewSymbology();
    setupViewDisplay();

    connectSlots();

    uiInner->lineEditDefaultSettings->setReadOnly(true);
    uiInner->lineEditActiveSettings->setReadOnly(true);

    uiInner->lineEditDefaultSettings->setText(situationalDisplay->getSettings()->SETTINGS_DFLT_PATH);
    uiInner->lineEditActiveSettings->setText(situationalDisplay->getSettings()->SETTINGS_ACTIVE_PATH);
}

DialogSettings::~DialogSettings()
{
    delete uiInner;
    if(modelSymbology != nullptr) delete modelSymbology;
    if(modelDisplay != nullptr) delete modelDisplay;
}

void DialogSettings::slotColorPickerClosed()
{
    flagDialogColorPickerExists = false;
}

void DialogSettings::slotUpdateTableColorARTCCLow(QColor color)
{
    QBrush brush(color);
    modelSymbology->item(0, 1)->setBackground(brush);
    modelSymbology->item(0, 1)->setForeground(brush);

    situationalDisplay->getSettings()->ARTCC_LOW_COLOR = color;
}

void DialogSettings::slotUpdateTableColorARTCCHigh(QColor color)
{
    QBrush brush(color);
    modelSymbology->item(1, 1)->setBackground(brush);
    modelSymbology->item(1, 1)->setForeground(brush);

    situationalDisplay->getSettings()->ARTCC_HIGH_COLOR = color;
}

void DialogSettings::slotUpdateTableColorARTCC(QColor color)
{
    QBrush brush(color);
    modelSymbology->item(2, 1)->setBackground(brush);
    modelSymbology->item(2, 1)->setForeground(brush);

    situationalDisplay->getSettings()->ARTCC_COLOR = color;
}

void DialogSettings::onTableClicked(const QModelIndex &index)
{
    QVariant value = index.data();

    if((value.toString() == "ARTCC Low Color") && !flagDialogColorPickerExists)
    {
        constructColorPicker(situationalDisplay->getSettings()->ARTCC_LOW_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorSectorARTCCLow(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateTableColorARTCCLow(QColor)));
    }
    else if((value.toString() == "ARTCC High Color") && !flagDialogColorPickerExists)
    {
        constructColorPicker(situationalDisplay->getSettings()->ARTCC_HIGH_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorSectorARTCCHigh(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateTableColorARTCCHigh(QColor)));
    }
    else if((value.toString() == "ARTCC Color") && !flagDialogColorPickerExists)
    {
        constructColorPicker(situationalDisplay->getSettings()->ARTCC_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorSectorARTCC(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateTableColorARTCC(QColor)));
    }
}

void DialogSettings::setupViewSymbology()
{
    uiInner->tableView->setModel(modelSymbology);
    uiInner->tableView->setGridStyle(Qt::NoPen);

    int rowHeight = 25;
    for(int i = 0; i < modelSymbology->rowCount(); i++)
    {
        uiInner->tableView->setRowHeight(i, rowHeight);
    }

    uiInner->tableView->setColumnWidth(0, 300);
    uiInner->tableView->setColumnWidth(1, 176);

    uiInner->tableView->horizontalHeader()->setHidden(true);
    uiInner->tableView->verticalHeader()->setHidden(true);

    connect(uiInner->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));
}

void DialogSettings::setupViewDisplay()
{
    uiInner->treeView->setModel(modelDisplay);
    uiInner->treeView->setHeaderHidden(true);

    uiInner->treeView->setColumnWidth(0, 375);
    uiInner->treeView->setColumnWidth(1, 50);
}

void DialogSettings::createModelSymbology()
{
    modelSymbology = new QStandardItemModel(0, 2, this);

    modelSymbology->appendRow(createSymbologyRow("ARTCC Low", situationalDisplay->getSettings()->ARTCC_LOW_COLOR));
    modelSymbology->appendRow(createSymbologyRow("ARTCC High", situationalDisplay->getSettings()->ARTCC_HIGH_COLOR));
    modelSymbology->appendRow(createSymbologyRow("ARTCC", situationalDisplay->getSettings()->ARTCC_COLOR));
}

void DialogSettings::createModelDisplay()
{
    ATCAirspace* airspaceData = situationalDisplay->getAirspaceData();
    bool dummyDisplayFlag = true; //TO BE CHANGED!---------------------------

    modelDisplay = new QStandardItemModel(0, 2, this);

//    QList<QStandardItem*> headerSectorARTCCLow(createDisplayHeader("ARTCC Low"));

//    for(int i = 0; i < airspaceData->getSectorARTCCLowVectorSize(); i++)
//    {
//        headerSectorARTCCLow.at(0)->appendRow(createDisplayRow(airspaceData->getSectorARTCCLow(i)->getName(), dummyDisplayFlag));
//    }

//    modelDisplay->appendRow(headerSectorARTCCLow);

//    populateTreeModel("ARTCC Low", reinterpret_cast<QVector<ATCAbstractSector*>&>(airspaceData->getSectorARTCCLowVector()), modelDisplay, dummyDisplayFlag);

    populateTreeModelTemplate("ARTCC Low", airspaceData->getSectorARTCCLowVector(), modelDisplay, dummyDisplayFlag);
    populateTreeModelTemplate("ARTCC High", airspaceData->getSectorARTCCHighVector(), modelDisplay, dummyDisplayFlag);
    populateTreeModelTemplate("ARTCC", airspaceData->getSectorARTCCVector(), modelDisplay, dummyDisplayFlag);
    populateTreeModelTemplate("Fixes", airspaceData->getFixesVector(), modelDisplay, dummyDisplayFlag);
    populateTreeModelTemplate("NDBs", airspaceData->getNDBsVector(), modelDisplay, dummyDisplayFlag);
    populateTreeModelTemplate("VORs", airspaceData->getVORsVector(), modelDisplay, dummyDisplayFlag);
    populateTreeModelTemplate("Airports", airspaceData->getAirportsVector(), modelDisplay, dummyDisplayFlag);
    populateTreeModelTemplate("SID Symbols", airspaceData->getSIDSymbolsVector(), modelDisplay, dummyDisplayFlag);
    populateTreeModelTemplate("STAR Symbols", airspaceData->getSTARSymbolsVector(), modelDisplay, dummyDisplayFlag);
    populateTreeModelTemplate("Low Airways", airspaceData->getAirwayLowVector(), modelDisplay, dummyDisplayFlag);
    populateTreeModelTemplate("High Airways", airspaceData->getAirwayHighVector(), modelDisplay, dummyDisplayFlag);


//    QList<QStandardItem*> headerSectorARTCCHigh(createDisplayHeader("ARTCC High"));
//    modelDisplay->appendRow(headerSectorARTCCHigh);

//    headerSectorARTCCHigh.at(0)->appendRow(createDisplayRow("ARTCC High 1", false));

//    QList<QStandardItem*> headerSectorARTCC(createDisplayHeader("ARTCC"));
//    modelDisplay->appendRow(headerSectorARTCC);

//    for(int i = 0; i < 50; i++)
//    {
//        QString name("ARTCC " + QString::number(i));
//        headerSectorARTCC.at(0)->appendRow(createDisplayRow(name, true));
//    }
}

void DialogSettings::connectSlots()
{
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorARTCCLow(QColor)), this, SLOT(slotUpdateTableColorARTCCLow(QColor)));
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorARTCCHigh(QColor)), this, SLOT(slotUpdateTableColorARTCCHigh(QColor)));
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorARTCC(QColor)), this, SLOT(slotUpdateTableColorARTCC(QColor)));
}

void DialogSettings::constructColorPicker(QColor &initColor)
{
    QColor currentColor(initColor);

    dialogColorPicker = new DialogColorPicker(currentColor, this);
    dialogColorPicker->show();

    connect(dialogColorPicker, SIGNAL(signalColorPickerClosed()), this, SLOT(slotColorPickerClosed()));

    flagDialogColorPickerExists = true;
}

QList<QStandardItem *> DialogSettings::createSymbologyRow(QString text, QColor color)
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

QList<QStandardItem*> DialogSettings::createDisplayHeader(QString text)
{
    QList<QStandardItem*> rowHeader;

    QStandardItem *headerName = new QStandardItem(text);
    headerName->setFlags(Qt::NoItemFlags);
    rowHeader.append(headerName);

    QStandardItem *headerFiller = new QStandardItem();
    headerFiller->setFlags(Qt::NoItemFlags);
    rowHeader.append(headerFiller);

    return rowHeader;
}

QList<QStandardItem *> DialogSettings::createDisplayRow(QString text, bool checked)
{
    QList<QStandardItem*> rowDisplay;

    QStandardItem *name = new QStandardItem(text);
    name->setFlags(Qt::NoItemFlags);
    rowDisplay.append(name);

    QStandardItem *checkbox = new QStandardItem();
    checkbox->setEditable(false);
    checkbox->setSelectable(false);
    checkbox->setCheckable(true);

    if(checked)
    {
        checkbox->setCheckState(Qt::Checked);
    }
    else
    {
        checkbox->setCheckState(Qt::Unchecked);
    }

    rowDisplay.append(checkbox);

    return rowDisplay;
}

void DialogSettings::populateTreeModel(QString headerName, QVector<ATCAbstractSector*> &vector, QStandardItemModel *model, bool dummyDisplayFlag)
{
    QList<QStandardItem*> categoryHeader(createDisplayHeader(headerName));

    for(int i = 0; i < vector.size(); i++)
    {
        categoryHeader.at(0)->appendRow(createDisplayRow(vector.at(i)->getName(), dummyDisplayFlag));
    }

    model->appendRow(categoryHeader);
}

void DialogSettings::on_buttonExportSettings_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Export to..."), situationalDisplay->getSettings()->SETTINGS_EXPORT_PATH, tr("Text files(*.txt)"));
    if(filePath.isEmpty()) return;

    situationalDisplay->getSettings()->exportSettings(filePath);

    QMessageBox msgBox(this);
    msgBox.setText("Settings successfuly exported to: " + filePath);
    msgBox.exec();
}

void DialogSettings::on_buttonLoadSettings_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Load settings..."), situationalDisplay->getSettings()->SETTINGS_EXPORT_PATH, tr("Text files(*.txt)"));
    if(filePath.isEmpty()) return;

    situationalDisplay->getSettings()->loadSettings(filePath);

    uiInner->lineEditActiveSettings->setText(situationalDisplay->getSettings()->SETTINGS_ACTIVE_PATH);
}

void DialogSettings::on_buttonSetDefault_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Set as default config..."), situationalDisplay->getSettings()->SETTINGS_EXPORT_PATH, tr("Text files(*.txt)"));
    if(filePath.isEmpty()) return;

    situationalDisplay->getSettings()->setDefaultPath(filePath);

    QMessageBox msgBox(this);
    msgBox.setText("Default settings set to: " + filePath);
    msgBox.exec();

    uiInner->lineEditDefaultSettings->setText(situationalDisplay->getSettings()->SETTINGS_DFLT_PATH);
}

template <class T> void DialogSettings::populateTreeModelTemplate(QString headerName, QVector<T*> &vector, QStandardItemModel *model, bool dummyDisplayFlag)
{
    QList<QStandardItem*> categoryHeader(createDisplayHeader(headerName));

    for(int i = 0; i < vector.size(); i++)
    {
        categoryHeader.at(0)->appendRow(createDisplayRow(vector.at(i)->getName(), dummyDisplayFlag));
    }

    model->appendRow(categoryHeader);
}
