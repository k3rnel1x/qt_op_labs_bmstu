#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "entrypoint.h"
#include "logic.h"

#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <qmessagebox.h>
#include <QFileDialog>
#include <stdlib.h>
#include <string.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ctx = get_ctx();
    ui->regionInputTextField->setPlaceholderText("Input Region");
    ui->collumInputTextField->setPlaceholderText("Input Collum");

    clear_labels();
    // tests
    // ctx->filename = "/home/k3rnel1x/Programing/qt-op-labs-bmstu/russian_demography.csv";
    // on_loadButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
    del_ctx(ctx);
}

void MainWindow::on_loadButton_clicked()
{
    switch(perform_operation(LOAD_TABLE, ctx))
    {
    case ERROR:
        QMessageBox::critical(this, "Error", "RUNTIME_ERROR");
        clear_labels();
        return;

    case FILE_EMPTY:
        QMessageBox::critical(this, "Error", "FILE_EMPTY");
        clear_labels();
        return;

    case TABLE_EMPTY:
        QMessageBox::critical(this, "Error", "TABLE_EMPTY");
        clear_labels();
        return;

    case NO_FILE:
        QMessageBox::critical(this, "Error", "NO_FILE");
        clear_labels();
        return;

    case SUCCESS:
    case NAME_COLLUM:
        break;
    }

    ui->errorsLabel->setText( "Errors count: " + QString::number(ctx->errors_count));

    model = new QStandardItemModel(ctx->lines.count, 6);
    Vector* v = &ctx->lines;
    for(int i = 0; i < v->count; i++)
    {
        Line* line = v_item(v, i);

        model->setItem(i, 0, new QStandardItem( QString::number(i+1)  ));
        model->setItem(i, 1, new QStandardItem( QString::number(line->year)         ));
        model->setItem(i, 2, new QStandardItem( line->region ));
        model->setItem(i, 3, new QStandardItem( QString::number(line->natural_grow) ));
        model->setItem(i, 4, new QStandardItem( QString::number(line->birth_rate)   ));
        model->setItem(i, 5, new QStandardItem( QString::number(line->death_rate)   ));
        model->setItem(i, 6, new QStandardItem( QString::number(line->urbanization) ));
    }

    model->setHeaderData(0, Qt::Horizontal, "Number");
    model->setHeaderData(1, Qt::Horizontal, "Year");
    model->setHeaderData(2, Qt::Horizontal, "Region");
    model->setHeaderData(3, Qt::Horizontal, "Natural grow");
    model->setHeaderData(4, Qt::Horizontal, "Birth rate");
    model->setHeaderData(5, Qt::Horizontal, "Death rate");
    model->setHeaderData(6, Qt::Horizontal, "Urbanization");

    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void MainWindow::on_openButton_clicked()
{
    QByteArray value = qgetenv("USER");
    QString filename = QFileDialog::getOpenFileName(this,
                                            tr("Open CSV"), "/home/" + value, tr("*.csv"));
    if(filename.isEmpty())
        return;

    QByteArray filename_bytearray = filename.toUtf8();
    const char* c_str = filename_bytearray.constData();
    char* const_ptr = (char*)calloc(filename.length() + 1, sizeof(char));
    strcpy(const_ptr, c_str);
    ctx->filename = const_ptr;
    ui->fileNameLabel->setText(QString(strrchr(ctx->filename, '/') + 1));
}


void MainWindow::on_calcButton_clicked()
{
    QString region = ui->regionInputTextField->toPlainText();
    QByteArray  region_barray = region.toUtf8();
    const char* region_cstr = region_barray.constData();
    ctx->choised_region = region_cstr;

    QString collum = ui->collumInputTextField->toPlainText();
    QByteArray collum_barr = collum.toUtf8();
    char* collum_cstr = collum_barr.data();
    ctx->choised_collum = collum_cstr;

    switch(perform_operation(CALC_METRIX, ctx))
    {
    case ERROR:
        QMessageBox::critical(this, "Error", "RUNTIME_ERROR");
        return;

    case NO_FILE:
        QMessageBox::critical(this, "Error", "NO_FILE");
        return;

    case EMPTY_REGION:
        QMessageBox::critical(this, "Error", "EMPTY_REGION");
        return;

    case NO_REGION:
        QMessageBox::critical(this, "Error", "NO_REGION");
        return;

    case EMPTY_COLLUM:
        QMessageBox::critical(this, "Error", "EMPTY_COLLUM");
        return;

    case INVALID_COLLUM:
        QMessageBox::critical(this, "Error", "INVALID_COLLUM");
        return;

    case NAME_COLLUM:
        QMessageBox::critical(this, "Error", "NAME_COLLUM");
        return;

    case OUTRANGE_COLLUM:
        QMessageBox::critical(this, "Error", "OUTRANGE_COLLUM");
        return;

    case SUCCESS:
        break;
    }

    ui->minTextField->setText(QString::number(ctx->min));
    ui->maxTextField->setText(QString::number(ctx->max));
    ui->midTextField->setText(QString::number(ctx->mid));
}

void MainWindow::clear_labels()
{
    ui->errorsLabel->clear();
    ui->fileNameLabel->setText("No file selected");
}
