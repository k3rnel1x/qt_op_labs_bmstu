#include "./mainwindow.h"
#include "./ui_mainwindow.h"

#include "logger.hpp"
#include <string.h>

#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <entrypoint.h>
#include <businesslogic.h>
#include <QtMinMax>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    context = new AppContext{};

    // Inizialize UI
    ui->configureRenderUI->setVisible(false);
    ui->maxSpinBox->setMinimum(1);
    connect(ui->minSpinBox, &QSpinBox::valueChanged, this, &MainWindow::on_spinboxes_valueChanged);
    connect(ui->maxSpinBox, &QSpinBox::valueChanged, this, &MainWindow::on_spinboxes_valueChanged);
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(&drawer);
    // drawer.setParent(ui->widget_2);
    ui->widget_2->setLayout(layout);
    drawer.setVisible(false);
}

MainWindow::~MainWindow()
{
    Logger::get_instance().logDebug("Start clearing..");

    performOperation(context, NULL, CleanUp);

    Logger::get_instance().logDebug("CleanUp SUCCEED");

    delete context;
    delete ui;
}

void MainWindow::on_loadDataButton_clicked()
{
    Logger::get_instance().logDebug("Loading file.. (loadDataButton clicked)");

    QString QfilenamePath = QFileDialog::getOpenFileName(
        this,
        tr("Open data"),
        DIRECTORY,
        tr("Table (*.csv)")
    );

    if(QfilenamePath.isEmpty()){
        drawer.setVisible(false);
        Logger::get_instance().logDebug("No file selected");
        return;
    }

    const char* charFilename = qstrtoc(QfilenamePath);
    Params prms = { .filenamePath = charFilename };
    
    ResultCode result = performOperation(context, &prms, LoadFile);
    if(result == SUCCEED) {
        Logger::get_instance().logDebug("LoadFile SUCCEED");
        this->updateConfigureUI();
        Logger::get_instance().logDebug(QString("Filename Selected: ") + QString(context->filenamePath));

        // set visible configureRenderUI
        ui->configureRenderUI->setVisible(true);
        ui->loadDataButton->setText(strrchr(charFilename, '/')+1);

    } else {
        handleResult(result);
        Logger::get_instance().logDebug("LoadFile Failed");

        // hide configureRenderU
        ui->configureRenderUI->setVisible(false);
        ui->loadDataButton->setText(DEFAULTLOADTEXT);

    }

    // hide render
    drawer.setVisible(false);
}

void MainWindow::on_renderButton_clicked()
{
    Logger::get_instance().logDebug("renderButton clicked");

    ResultCode result = performOperation(context, NULL, NormalizePoints);
    if(result != SUCCEED) {
        // hide render
        drawer.setVisible(false);
        handleResult(result);
        return;
    }

    PointsArr* arr = &context->points;
    drawer.updateData(arr);
    drawer.setVisible(true);
}

void MainWindow::on_stepSlider_valueChanged()
{
    Logger::get_instance().logDebug("stepSlider valueChanged");

    size_t currStep = ui->stepSlider->sliderPosition();
    Params prms = { .renderStep = currStep };
    
    ResultCode result = performOperation(context, &prms, UpdateStepConfig);
    if(result != SUCCEED) {
        handleResult(result);
        return;
    }

    ui->currStepLabel->setText(QString::number(currStep));
}

void MainWindow::on_spinboxes_valueChanged()
{
    Logger::get_instance().logDebug("spinboxes valueChanged");

    int maxValue = ui->maxSpinBox->value();
    int minValue = ui->minSpinBox->value();
    ui->minSpinBox->setMaximum(qMax<int, int>(maxValue - 1, 0));

    Params prms = { .maxNormalizationRange = maxValue, .minNormalizationRange = minValue };
    ResultCode result = performOperation(context, &prms, UpdateRangeConfig);
    if(result != SUCCEED) {
        handleResult(result);
        return;
    }
}

void MainWindow::handleResult(ResultCode result)
{
    if (result == SUCCEED) return;

    const char* err_text = NULL;
    switch(result)
    {
        case ERROR:
            err_text = "RUNTIME_ERROR";
            break;

        case EMPTY_FILE:
            err_text = "EMPTY_FILE";
            break;

        case NO_FILE:
            err_text = "NO_FILE";
            break;

        case WRONG_FILE_EXTENSION:
            err_text = "WRONG_FILE_EXTENSION(MUST BE .CSV)";
            break;

        case INVALIDTABLE:
            err_text = "INVALID TABLE!";
            break;

        case RANGESAREINDENTICAL:
            err_text = "RANGES ARE INDENTICAL!";
            break;
    }

    QMessageBox::critical(this, "Error", err_text);
}

void MainWindow::updateConfigureUI()
{
    // update step labels
    ui->minStepLabel->setText(QString::number(context->minStep));
    ui->maxStepLabel->setText(QString::number(context->maxStep));
    size_t defaultStep = (context->maxStep - context->minStep) / 2;
    ui->currStepLabel->setText(QString::number(defaultStep));

    // update slider range
    QSlider* slider = ui->stepSlider;
    slider->setTickInterval(1);
    slider->setRange(context->minStep, context->maxStep);
    slider->setValue(defaultStep);
    slider->setTickPosition(QSlider::TickPosition::TicksBelow);

    // update QSpinBoxes
    ui->minSpinBox->setValue(0);
    ui->maxSpinBox->setValue(1);

    // update context
    on_stepSlider_valueChanged();
    on_spinboxes_valueChanged();
}

char* MainWindow::qstrtoc(const QString& qstr)
{
    QByteArray bytearray = qstr.toUtf8();
    const char* c_str = bytearray.constData();

    return strdup(c_str);
}