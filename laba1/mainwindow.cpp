#include <iostream>
#include <qabstractbutton.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qstringview.h>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	// Init context
    ctx = (AppContext*)calloc(1, sizeof(AppContext));
    doOperation(INIT, ctx);

	// Group RadioButtons
    inputRadioButtons  = new QButtonGroup(this);
	outputRadioButtons = new QButtonGroup(this);
    groupRadioButtons();


	// Create connections
    connect(
    ui->convertButton, &QPushButton::clicked,
                 this, &MainWindow::on_convertButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    doOperation(DEINIT, ctx);
}

/* Getters */
const char* MainWindow::getInputText()
{
    QString qtext = ui->inputNumberTextField->toPlainText();
    QByteArray qbytes = qtext.toUtf8();
    const char* ctext = qbytes.constData();
    return ctext;
}

const char* MainWindow::getCustomInputSystem()
{
    QString qtext = ui->inputCustomSystemField->toPlainText();
    QByteArray qbytes = qtext.toUtf8();
    const char* ctext = qbytes.constData();
    return ctext;
}

const char* MainWindow::getCustomOutputSystem()
{
    QString qtext = ui->getCustomOutputSystem->toPlainText();
    QByteArray qbytes = qtext.toUtf8();
    const char* ctext = qbytes.constData();
    return ctext;
}

/* Setters */
void MainWindow::updateOutText()
{
    ui->outputNumberTextField->setPlainText(ctx->outputText);
}


void MainWindow::groupRadioButtons()
{
    // Init input radio-buttons
    inputRadioButtons->addButton(ui->radioButtonTwoInput,   0);
    inputRadioButtons->addButton(ui->radioButtonEightInput, 1);
    inputRadioButtons->addButton(ui->radioButtonTenInput,   2);
    inputRadioButtons->addButton(ui->radioButtonCustomSystemInput,   3);

    // Init output radio-buttons
    outputRadioButtons->addButton(ui->radioButtonTwoOutput,   0);
    outputRadioButtons->addButton(ui->radioButtonEightOutput, 1);
    outputRadioButtons->addButton(ui->radioButtonTenOutput,   2);
    outputRadioButtons->addButton(ui->radioButtonCustomSystemOutput,   3);
}

/* Slots */
void MainWindow::getNumSystems(AppContext* context)
{
    // TODO optimize and do better
    int inputIdx = inputRadioButtons->checkedId();
    if(inputIdx == 3) {
        if(context->customInputSystem != NULL)
        {
            free(context->customInputSystem);
            context->customInputSystem = NULL;
        }
        context->customInputSystem = getCustomInputSystem();
    }
    context->checkedInputRadioButton = inputIdx;

    int outputIdx = inputRadioButtons->checkedId();
    if(outputIdx == 3) {
        if(context->customOutputSystem != NULL)
        {
            free(context->customOutputSystem);
            context->customOutputSystem = NULL;
        }
        context->customOutputSystem = getCustomOutputSystem();
    }
    context->checkedOutputRadioButton = outputIdx;
}

void MainWindow::on_convertButton_clicked()
{
    ctx->inputText = getInputText();

    getNumSystems(ctx);
    doOperation(CONVERT, ctx);
    setOutText();
    doOperation(CLEAR, ctx);
}


void MainWindow::on_CopyToClipboardLeftButton_clicked()
{

}


void MainWindow::on_CopyToClipboardRightButton_clicked()
{

}


void MainWindow::on_swapNumSystemButton_clicked()
{

}

