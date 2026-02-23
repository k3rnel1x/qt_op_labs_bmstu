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
	doOperation(Initialization, ctx);

	// Group RadioButtons
    inputRadioButtons  = new QButtonGroup(this);
	outputRadioButtons = new QButtonGroup(this);
    initRadioButtons();


	// Create connections
    connect(ui->convertButton, &QPushButton::clicked,
                         this, &MainWindow::on_convertButton_clicked);

	connect(inputRadioButtons, &QButtonGroup::idClicked,
                         this, &MainWindow::on_inputRadioButtons_clicked);

	connect(outputRadioButtons, &QButtonGroup::idClicked,
                          this, &MainWindow::on_outputRadioButtons_clicked);

}

MainWindow::~MainWindow()
{
    delete ui;
    doOperation(DeInitialization, ctx);
}

// Getters
const char* MainWindow::getInputText()
{
    QString qtext = ui->inputNumberTextField->toPlainText();
    QByteArray qbytes = qtext.toUtf8();
    const char* ctext = qbytes.constData();
    return ctext;
}

// Setters
void MainWindow::updateOutText()
{
    ui->outputNumberTextField->setPlainText(ctx->outputText);
}


void MainWindow::initRadioButtons()
{
    // Init input radio-buttons
    inputRadioButtons->addButton(ui->radioButtonTwoInput,   0);
    inputRadioButtons->addButton(ui->radioButtonEightInput, 1);
    inputRadioButtons->addButton(ui->radioButtonTenInput,   2);

    // Init output radio-buttons
    outputRadioButtons->addButton(ui->radioButtonTwoOutput,   0);
    outputRadioButtons->addButton(ui->radioButtonEightOutput, 1);
    outputRadioButtons->addButton(ui->radioButtonTenOutput,   2);
}

// Slots

void MainWindow::on_inputRadioButtons_clicked()
{
    NumSystem num = TEN;
    int idx = inputRadioButtons->checkedId(); 
    switch(idx)
    {
    case 0:
        num = TWO;
        break;
    case 1:
        num = TEN;
        break;
    case 2:
        num = SXTEEN;
        break;
    }
    ctx->iptsys = num;
    qDebug() << ctx->iptsys;
}


void MainWindow::on_outputRadioButtons_clicked()
{
    NumSystem num;
    int idx = outputRadioButtons->checkedId();
    switch(idx)
    {
    case 0:
        num = TWO;
        break;
    case 1:
        num = TEN;
        break;
    case 2:
        num = SXTEEN;
        break;
    }
    ctx->outsys = num;
}

void MainWindow::on_convertButton_clicked()
{
    if(ctx->inputText != NULL)
        free(ctx->inputText);
    ctx->inputText = getInputText();

    doOperation(Convert, ctx);
    updateOutText();
}


void MainWindow::on_CopyToClipboardLeftButton_clicked()
{

}


void MainWindow::on_CopyToClipboardRightButton_clicked()
{

}

