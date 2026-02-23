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
    inputRadioButtons = new QButtonGroup(this);
	inputRadioButtons->addButton(ui->radioButton1Ipt, 0);	
	inputRadioButtons->addButton(ui->radioButton2Ipt, 1);	
	inputRadioButtons->addButton(ui->radioButton3Ipt, 2);	

	outputRadioButtons = new QButtonGroup(this);
	outputRadioButtons->addButton(ui->radioButton1Out, 0);	
	outputRadioButtons->addButton(ui->radioButton2Out, 1);	
	outputRadioButtons->addButton(ui->radioButton3Out, 2);	

	// Create connections
    connect(ui->convertButton, &QPushButton::clicked,
            this, &MainWindow::onPushConvertButton);

	connect(inputRadioButtons, &QButtonGroup::idClicked,
            this, &MainWindow::UpdateLSystem);
	connect(outputRadioButtons, &QButtonGroup::idClicked,
            this, &MainWindow::UpdateRSystem);



}

MainWindow::~MainWindow()
{
    delete ui;
    doOperation(DeInitialization, ctx);
}

void MainWindow::onPushConvertButton()
{
	getInputText();
    doOperation(Convert, ctx);
    updateOutText();
}

void MainWindow::updateOutText()
{
    ui->outputTextField->setPlainText(ctx->outputText);
}
void MainWindow::getInputText()
{
	QString qtext = ui->inputTextField->toPlainText();
	QByteArray qbytes = qtext.toUtf8();
	const char* ctext = qbytes.constData();
	strcpy(ctx->inputText, ctext);
}

void MainWindow::initRadioButtons()
{

}

void MainWindow::UpdateLSystem()
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

void MainWindow::UpdateRSystem()
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

}


void MainWindow::on_CopyToClipboardLeftButton_clicked()
{

}


void MainWindow::on_CopyToClipboardRightButton_clicked()
{

}


void MainWindow::on_pushButton_clicked()
{

}

