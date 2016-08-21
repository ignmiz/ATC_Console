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

void DialogSettings::onTreeViewClicked(const QModelIndex &index)
{
    bool isNotHeader(index.parent().isValid());

    if(isNotHeader)
    {
        QModelIndex checkboxIndex(index.model()->index(index.row(), 1, index.parent()));
        QModelIndex nameIndex(index.model()->index(index.row(), 0, index.parent()));

        QStandardItem *checkboxItem(modelDisplay->itemFromIndex(checkboxIndex));
        bool isChecked((checkboxItem->checkState()) == Qt::Checked);

        QString parentHeaderName(index.parent().data().toString());
        QString rowName(nameIndex.data().toString());

        if(isChecked)
        {
            checkboxItem->setCheckState(Qt::Unchecked);

            if(parentHeaderName == "Sectors: ARTCC Low")
            {
                emit signalHideSectorARTCCLow(rowName);
            }
            else if(parentHeaderName == "Sectors: ARTCC High")
            {
                emit signalHideSectorARTCCHigh(rowName);
            }
            else if(parentHeaderName == "Sectors: ARTCC")
            {
                emit signalHideSectorARTCC(rowName);
            }
            else if(parentHeaderName == "Fixes")
            {
                emit signalHideFix(rowName);
            }
            else if(parentHeaderName == "Beacons: NDB")
            {
                emit signalHideNDB(rowName);
            }
            else if(parentHeaderName == "Beacons: VOR")
            {
                emit signalHideVOR(rowName);
            }
            else if(parentHeaderName == "Airports")
            {
                emit signalHideAirport(rowName);
            }
            else if(parentHeaderName == "Procedures: SID")
            {
                emit signalHideSID(rowName);
            }
            else if(parentHeaderName == "Procedures: STAR")
            {
                emit signalHideSTAR(rowName);
            }
            else if(parentHeaderName == "Airways: Low")
            {
                emit signalHideAirwayLow(rowName);
            }
            else if(parentHeaderName == "Airways: High")
            {
                emit signalHideAirwayHigh(rowName);
            }
        }
        else
        {
            checkboxItem->setCheckState(Qt::Checked);

            if(parentHeaderName == "Sectors: ARTCC Low")
            {
                emit signalShowSectorARTCCLow(rowName);
            }
            else if(parentHeaderName == "Sectors: ARTCC High")
            {
                emit signalShowSectorARTCCHigh(rowName);
            }
            else if(parentHeaderName == "Sectors: ARTCC")
            {
                emit signalShowSectorARTCC(rowName);
            }
            else if(parentHeaderName == "Fixes")
            {
                emit signalShowFix(rowName);
            }
            else if(parentHeaderName == "Beacons: NDB")
            {
                emit signalShowNDB(rowName);
            }
            else if(parentHeaderName == "Beacons: VOR")
            {
                emit signalShowVOR(rowName);
            }
            else if(parentHeaderName == "Airports")
            {
                emit signalShowAirport(rowName);
            }
            else if(parentHeaderName == "Procedures: SID")
            {
                emit signalShowSID(rowName);
            }
            else if(parentHeaderName == "Procedures: STAR")
            {
                emit signalShowSTAR(rowName);
            }
            else if(parentHeaderName == "Airways: Low")
            {
                emit signalShowAirwayLow(rowName);
            }
            else if(parentHeaderName == "Airways: High")
            {
                emit signalShowAirwayHigh(rowName);
            }
        }
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
    connect(uiInner->treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTreeViewClicked(const QModelIndex &)));
}

void DialogSettings::setupViewDisplay()
{
    uiInner->treeView->setModel(modelDisplay);
    uiInner->treeView->setHeaderHidden(true);
    uiInner->treeView->sortByColumn(0, Qt::AscendingOrder);

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

    modelDisplay = new QStandardItemModel(0, 2, this);

    populateTreeModel("Sectors: ARTCC Low", airspaceData->getSectorARTCCLowVector(), modelDisplay);
    populateTreeModel("Sectors: ARTCC High", airspaceData->getSectorARTCCHighVector(), modelDisplay);
    populateTreeModel("Sectors: ARTCC", airspaceData->getSectorARTCCVector(), modelDisplay);
    populateTreeModel("Fixes", airspaceData->getFixesVector(), modelDisplay);
    populateTreeModel("Beacons: NDB", airspaceData->getNDBsVector(), modelDisplay);
    populateTreeModel("Beacons: VOR", airspaceData->getVORsVector(), modelDisplay);
    populateTreeModel("Airports", airspaceData->getAirportsVector(), modelDisplay);
    populateTreeModel("Procedures: SID", airspaceData->getSIDSymbolsVector(), modelDisplay);
    populateTreeModel("Procedures: STAR", airspaceData->getSTARSymbolsVector(), modelDisplay);
    populateTreeModel("Airways: Low", airspaceData->getAirwayLowVector(), modelDisplay);
    populateTreeModel("Airways: High", airspaceData->getAirwayHighVector(), modelDisplay);
}

void DialogSettings::connectSlots()
{
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorARTCCLow(QColor)), this, SLOT(slotUpdateTableColorARTCCLow(QColor)));
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorARTCCHigh(QColor)), this, SLOT(slotUpdateTableColorARTCCHigh(QColor)));
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorARTCC(QColor)), this, SLOT(slotUpdateTableColorARTCC(QColor)));

    connect(this, SIGNAL(signalHideSectorARTCCLow(QString)), situationalDisplay, SLOT(slotHideSectorARTCCLow(QString)));
    connect(this, SIGNAL(signalHideSectorARTCCHigh(QString)), situationalDisplay, SLOT(slotHideSectorARTCCHigh(QString)));
    connect(this, SIGNAL(signalHideSectorARTCC(QString)), situationalDisplay, SLOT(slotHideSectorARTCC(QString)));
    connect(this, SIGNAL(signalHideFix(QString)), situationalDisplay, SLOT(slotHideFix(QString)));
    connect(this, SIGNAL(signalHideNDB(QString)), situationalDisplay, SLOT(slotHideNDB(QString)));
    connect(this, SIGNAL(signalHideVOR(QString)), situationalDisplay, SLOT(slotHideVOR(QString)));
    connect(this, SIGNAL(signalHideAirport(QString)), situationalDisplay, SLOT(slotHideAirport(QString)));
    connect(this, SIGNAL(signalHideSID(QString)), situationalDisplay, SLOT(slotHideSID(QString)));
    connect(this, SIGNAL(signalHideSTAR(QString)), situationalDisplay, SLOT(slotHideSTAR(QString)));
    connect(this, SIGNAL(signalHideAirwayLow(QString)), situationalDisplay, SLOT(slotHideAirwayLow(QString)));
    connect(this, SIGNAL(signalHideAirwayHigh(QString)), situationalDisplay, SLOT(slotHideSTAR(QString)));

    connect(this, SIGNAL(signalShowSectorARTCCLow(QString)), situationalDisplay, SLOT(slotShowSectorARTCCLow(QString)));
    connect(this, SIGNAL(signalShowSectorARTCCHigh(QString)), situationalDisplay, SLOT(slotShowSectorARTCCHigh(QString)));
    connect(this, SIGNAL(signalShowSectorARTCC(QString)), situationalDisplay, SLOT(slotShowSectorARTCC(QString)));
    connect(this, SIGNAL(signalShowFix(QString)), situationalDisplay, SLOT(slotShowFix(QString)));
    connect(this, SIGNAL(signalShowNDB(QString)), situationalDisplay, SLOT(slotShowNDB(QString)));
    connect(this, SIGNAL(signalShowVOR(QString)), situationalDisplay, SLOT(slotShowVOR(QString)));
    connect(this, SIGNAL(signalShowAirport(QString)), situationalDisplay, SLOT(slotShowAirport(QString)));
    connect(this, SIGNAL(signalShowSID(QString)), situationalDisplay, SLOT(slotShowSID(QString)));
    connect(this, SIGNAL(signalShowSTAR(QString)), situationalDisplay, SLOT(slotShowSTAR(QString)));
    connect(this, SIGNAL(signalShowAirwayLow(QString)), situationalDisplay, SLOT(slotShowAirwayLow(QString)));
    connect(this, SIGNAL(signalShowAirwayHigh(QString)), situationalDisplay, SLOT(slotShowSTAR(QString)));
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
    checkbox->setCheckable(false);

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

template<class T> void DialogSettings::populateTreeModel(QString headerName, QVector<T*> const &vector, QStandardItemModel *model)
{
    QList<QStandardItem*> categoryHeader(createDisplayHeader(headerName));

    for(int i = 0; i < vector.size(); i++)
    {
        categoryHeader.at(0)->appendRow(createDisplayRow(vector.at(i)->getName(), vector.at(i)->isVisible()));
    }

    model->appendRow(categoryHeader);
}
