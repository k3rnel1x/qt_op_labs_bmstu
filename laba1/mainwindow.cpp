#include <iostream>
#include <qabstractbutton.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qstringview.h>
#include <string.h>
#include "mainwindow.h"
#include "errors.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Init (context)
    ctx = (AppContext*)calloc(1, sizeof(AppContext));
    doOperation(INIT, ctx);

    // Group and Init RadioButtons
    inputRadioButtons  = new QButtonGroup(this);
	outputRadioButtons = new QButtonGroup(this);
    groupRadioButtons();

    ui->radioButtonTenInput->setChecked (1);
    ui->radioButtonTwoOutput->setChecked(1);

    ui->inputCustomSystemField->setEnabled (0);
    ui->outputCustomSystemField->setEnabled(0);

	// Create connections
    // connect(
    // ui->convertButton, &QPushButton::clicked,
    //              this, &MainWindow::on_convertButton_clicked);
    // fuk ths sht

    connect(
    inputRadioButtons, &QButtonGroup::buttonClicked,
                 this, &MainWindow::on_radioButtonCustomSystemInput_clicked);

    connect(
    outputRadioButtons, &QButtonGroup::buttonClicked,
                  this, &MainWindow::on_radioButtonCustomSystemOutput_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    doOperation(DEINIT, ctx);
}

/* Getters */
char* MainWindow::getInputText()
{
    QString qtext = ui->inputNumberTextField->toPlainText();
    QByteArray qbytes = qtext.toLocal8Bit();
    char* ctxt = qbytes.data();
    char* str = (char*)calloc(strlen(ctxt)+1, sizeof(char));
    strcpy(str, ctxt);
    return str;
}

char* MainWindow::getCustomInputSystem()
{
    QString qtext = ui->inputCustomSystemField->toPlainText();
    QByteArray qbytes = qtext.toLocal8Bit();
    char* ctxt = qbytes.data();
    char* str = (char*)calloc(strlen(ctxt)+1, sizeof(char));
    strcpy(str, ctxt);
    return str;
}

char* MainWindow::getCustomOutputSystem()
{
    QString qtext = ui->outputCustomSystemField->toPlainText();
    QByteArray qbytes = qtext.toLocal8Bit();
    char* ctxt = qbytes.data();
    char* str = (char*)calloc(strlen(ctxt)+1, sizeof(char));
    strcpy(str, ctxt);
    return str;
}

/* Setters */
void MainWindow::setOutText(AppContext* context)
{
    ui->outputNumberTextField->setPlainText(context->outputText);
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

    int outputIdx = outputRadioButtons->checkedId();
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
    // qDebug() << "on_convertButton_clicked executed";
    ctx->inputText = getInputText();

    getNumSystems(ctx);
    Result convertRes = doOperation(CONVERT, ctx);
    handleResult(convertRes);

    setOutText(ctx);
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

void MainWindow::on_radioButtonCustomSystemInput_clicked()
{
    int idx = inputRadioButtons->checkedId();
    if(idx == 3)
        ui->inputCustomSystemField->setEnabled(1);
    else
        ui->inputCustomSystemField->setEnabled(0);
}

void MainWindow::on_radioButtonCustomSystemOutput_clicked()
{
    int idx = outputRadioButtons->checkedId();
    if(idx == 3)
        ui->outputCustomSystemField->setEnabled(1);
    else
        ui->outputCustomSystemField->setEnabled(0);
}

// Utils
void MainWindow::handleResult(Result res)
{
    switch (res) {
    case INPUT_ERROR:
        QMessageBox::critical(this, "Error", "Bad input in input field");
        break;

    case NUM_RANGE_ERROR:
        QMessageBox::critical(this, "Error", "Bad num range input");
        break;

    case INPUT_ALPHABET_ERROR:
        QMessageBox::critical(this, "Error", "Bad alphabet in input");
        break;

    case SUCCEED:
        QMessageBox::information(this, "Succeed", "Succeed convert");
        break;
    }
}
