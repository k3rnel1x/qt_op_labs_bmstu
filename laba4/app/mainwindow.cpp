#include "./mainwindow.h"
#include "./ui_mainwindow.h"

#include "logger.hpp"

#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <entrypoint.h>
#include <businesslogic.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    context = new AppContext{};
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
    }

    if(result != SUCCEED) {
        handleResult(result);
        Logger::get_instance().logDebug("LoadFile Failed");
        return;
    }
}

void MainWindow::on_renderButton_clicked()
{
    
}

void MainWindow::on_stepSlider_valueChanged()
{
    size_t currStep = ui->stepSlider->sliderPosition();
    Params prms = { .renderStep = currStep };
    
    ResultCode result = performOperation(context, &prms, UpdateRenderConfig);
    if(result != SUCCEED) {
        handleResult(result);
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
}

char* MainWindow::qstrtoc(const QString& qstr)
{
    QByteArray bytearray = qstr.toUtf8();
    const char* c_str = bytearray.constData();

    return strdup(c_str);
}