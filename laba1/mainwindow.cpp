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

    // Init Context
    ctx = (AppContext*)calloc(1, sizeof(AppContext));
    doOperation(INIT, ctx);
    // Init Clipboard
    clipboard = QGuiApplication::clipboard();

    // Init Text fields
    setOutputText(ctx);
    setInputText(ctx);

    ui->inputCustomSystemField->setEnabled (0);
    ui->outputCustomSystemField->setEnabled(0);

    // Init Radio buttons
    inputRadioButtons  = new QButtonGroup(this);
    outputRadioButtons = new QButtonGroup(this);
    initRadioButtons();

	// Create connections
    // connect(
    // ui->convertButton, &QPushButton::clicked,
    //              this, &MainWindow::on_convertButton_clicked);

    // connect(
    // ui->CopyToClipboardLeftButton, &QPushButton::clicked,
    //              this, &MainWindow::on_CopyToClipboardLeftButton_clicked);

    // connect(
    // ui->CopyToClipboardRightButton, &QPushButton::clicked,
    //              this, &MainWindow::on_CopyToClipboardRightButton_clicked);
    // // fuk ths sht

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
void MainWindow::getInputText(AppContext* context)
{
    QString qtext = ui->inputNumberTextField->toPlainText();
    QByteArray qbytes = qtext.toLocal8Bit();
    char* ctxt = qbytes.data();
    char* str = (char*)calloc(strlen(ctxt)+1, sizeof(char));
    strcpy(str, ctxt);

    if(context->inputText != NULL)
        free(context->inputText);

    context->inputText = str;
    // qDebug() << context->inputText;
}
void MainWindow::getCustomInputSystem(AppContext* context)
{
    QString qtext = ui->inputCustomSystemField->toPlainText();
    QByteArray qbytes = qtext.toLocal8Bit();
    char* ctxt = qbytes.data();
    char* str = (char*)calloc(strlen(ctxt)+1, sizeof(char));
    strcpy(str, ctxt);

    if(context->customInputSystem != NULL)
        free(context->customInputSystem);

    context->customInputSystem = str;
}

void MainWindow::getCustomOutputSystem(AppContext* context)
{
    QString qtext = ui->outputCustomSystemField->toPlainText();
    QByteArray qbytes = qtext.toLocal8Bit();
    char* ctxt = qbytes.data();
    char* str = (char*)calloc(strlen(ctxt)+1, sizeof(char));
    strcpy(str, ctxt);

    if(context->customOutputSystem != NULL)
        free(context->customOutputSystem);

    context->customOutputSystem = str;
}

/* Setters */
void MainWindow::setOutputText(AppContext* context)
{
    ui->outputNumberTextField->setPlainText(context->outputText);
}

void MainWindow::setInputText(AppContext* context)
{
    ui->inputNumberTextField->setPlainText(context->inputText);
}

void MainWindow::setInputSystemText(AppContext* context)
{
    ui->inputCustomSystemField->setPlainText(context->customInputSystem);
}

void MainWindow::setOutputSystemText(AppContext* context)
{
    ui->outputCustomSystemField->setPlainText(context->customOutputSystem);
}

void MainWindow::initRadioButtons()
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

    inputRadioButtons ->button(ctx->checkedInputRadioButton) ->setChecked(1);
    outputRadioButtons->button(ctx->checkedOutputRadioButton)->setChecked(1);
}

/* Slots */


void MainWindow::on_convertButton_clicked()
{
    doOperation(CLEAR, ctx);
    getInputText(ctx);
    getNumSystems(ctx);

    Result convertRes = doOperation(CONVERT, ctx);
    // qDebug() << convertRes;
    handleResult(convertRes);

    setOutputText(ctx);
    setInputText(ctx);
    // doOperation(CLEAR, ctx);
}


void MainWindow::on_CopyToClipboardLeftButton_clicked()
{
    QString qtext = ui->inputNumberTextField->toPlainText();
    clipboard->setText(qtext);
}


void MainWindow::on_CopyToClipboardRightButton_clicked()
{
    QString qtext = ui->outputNumberTextField->toPlainText();
    clipboard->setText(qtext);
}

// TODO implement swap func: does not work
void MainWindow::on_swapNumSystemButton_clicked()
{
    // Get all to context
    getNumSystems(ctx);
    getInputText(ctx);
    doOperation(SWAP, ctx);

    // Set all from context
    inputRadioButtons->button(ctx->checkedInputRadioButton)->setChecked(1);
    outputRadioButtons->button(ctx->checkedOutputRadioButton)->setChecked(1);

    setInputSystemText (ctx);
    setOutputSystemText(ctx);
    setOutputText(ctx);
    setInputText (ctx);
    on_radioButtonCustomSystemInput_clicked();
    on_radioButtonCustomSystemOutput_clicked();
}

void MainWindow::on_radioButtonCustomSystemInput_clicked()
{
    int idx = inputRadioButtons->checkedId();
    ui->inputCustomSystemField->setEnabled(idx == 3);
}

void MainWindow::on_radioButtonCustomSystemOutput_clicked()
{
    int idx = outputRadioButtons->checkedId();
    ui->outputCustomSystemField->setEnabled(idx == 3);
}

/* Utils */
void MainWindow::handleResult(Result res)
{
    switch (res) {

    case ERROR:
        QMessageBox::critical(this, "Error", "Internal error");
        break;

    case INPUT_ALPHABET_ERROR:
        QMessageBox::critical(this, "Error", "Bad input alphabet");
        break;

    case OUTPUT_ALPHABET_ERROR:
        QMessageBox::critical(this, "Error", "Bad output alphabet");
        break;

    case INPUT_ERROR:
        QMessageBox::critical(this, "Error", "Bad input in input field");
        break;

    case NUM_RANGE_ERROR:
        QMessageBox::critical(this, "Error", "Bad num range input");
        break;

    case TOO_LARGE_NUM_ERROR:
        QMessageBox::critical(this, "Error", "Too large num in input");
        break;

    case SUCCEED:
        QMessageBox::information(this, "Succeed", "Succeed convert");
        break;


    }
}
// TODO fix: does not work
void MainWindow::getNumSystems(AppContext* context)
{
    int inputIdx = inputRadioButtons->checkedId();
    // qDebug() << "inputRadioButtons->checkedId() = " << inputRadioButtons->checkedId();
    if(inputIdx == 3)
        getCustomInputSystem(context);

    context->checkedInputRadioButton = inputIdx;

    int outputIdx = outputRadioButtons->checkedId();
    // qDebug() << "outputRadioButtons->checkedId() = " << outputRadioButtons->checkedId();
    if(outputIdx == 3)
        getCustomOutputSystem(context);
    context->checkedOutputRadioButton = outputIdx;
}
