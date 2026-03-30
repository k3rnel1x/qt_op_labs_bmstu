#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../entrypoint.h"
#include "../logic/appcontext.h"

#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <qmessagebox.h>
#include <QFileDialog>
#include <stdlib.h>
#include <string.h>
// TODO handle errors method
// TODO delete context method
// TODO pricolchiki
// TODO pricolchiki
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ctx = (AppContext*)calloc(1, sizeof(AppContext));

    ui->regionList->setEnabled(false);
    ui->collumList->setEnabled(false);

    update_filelabel();
}

MainWindow::~MainWindow()
{
    delete ui;

    // ### delete context ###
    if (ctx->table) {
        for (size_t i = 0; i < ctx->table_len; i++)
            free(ctx->table[i]);
        free(ctx->table);
    }

    if (ctx->filename)
        free((char*)ctx->filename);

    if (ctx->regions)
        free(ctx->regions);
}

void MainWindow::on_loadButton_clicked()
{
    // ctx->filename = "/home/k3rnel1x/Programing/qt-op-labs-bmstu/laba2/test_files/russian_demography.csv";

    if (ctx->table) {
        for (size_t i = 0; i < ctx->table_len; i++)
            free(ctx->table[i]);
        free(ctx->table);
    }
    ctx->table = NULL;

    switch(perform_operation(LOAD_TABLE, ctx))
    {
        case RUNTIME_ERROR:
        QMessageBox::critical(this, "Error", "RUNTIME_ERROR");
        update_filelabel();
        return;

    case EMPTY_FILE:
        QMessageBox::critical(this, "Error", "EMPTY_FILE");
        ctx->load_time = -1;
        update_filelabel();
        return;

    case NO_FILE:
    case NO_FILE_SELECTED:
        QMessageBox::critical(this, "Error", "NO_FILE");
        ctx->load_time = -1;
        update_filelabel();
        return;

    case INVALID_HEADER:
        QMessageBox::critical(this, "Error", "INVALID_HEADER");
        update_filelabel();
        return;

    case SUCCESS:
        break;
    }
    update_window_header();

    QMessageBox msgBox(this);
    msgBox.setText("Load time: " + QString::number(ctx->load_time));
    msgBox.exec();

    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(COLLUMS_COUNT);
    ui->tableWidget->setRowCount(ctx->table_len-1);

    char*** table = ctx->table;
    size_t table_len = ctx->table_len;

    // set header
    for (size_t c = 0; c < COLLUMS_COUNT; c++)
    {
        ui->tableWidget->setHorizontalHeaderItem(c, new QTableWidgetItem( table[0][c] ));
    }


    for (size_t r = 0; r < table_len-1; r++)
    {
        for (size_t c = 0; c < COLLUMS_COUNT; c++)
        {
            ui->tableWidget->setItem(r, c, new QTableWidgetItem( table[r+1][c] ));
            // qDebug() << table[r][c];
        }
    }

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
    ctx->filename = const_ptr; ui->fileNameLabel->setText(QString(strrchr(ctx->filename, '/') + 1));
}

void MainWindow::update_filelabel()
{
    if (!ctx->filename)
        ui->fileNameLabel->setText("No file selected");
    else
        ui->fileNameLabel->setText(ctx->filename);
}

void MainWindow::update_window_header()
{
    if (ctx->load_time == -1)
        this->setWindowTitle("LibreTassov");
    else
        this->setWindowTitle("Load time: " + QString::number(ctx->load_time));

}

// void MainWindow::on_calcButton_clicked()
// {
//     QString region = ui->regionInputTextField->toPlainText();
//     QByteArray  region_barray = region.toUtf8();
//     const char* region_cstr = region_barray.constData();
//     ctx->choised_region = region_cstr;
//
//     QString collum = ui->collumInputTextField->toPlainText();
//     QByteArray collum_barr = collum.toUtf8();
//     char* collum_cstr = collum_barr.data();
//     ctx->choised_collum = collum_cstr;
//
//     switch(perform_operation(CALC_METRIX, ctx))
//     {
//     case ERROR:
//         QMessageBox::critical(this, "Error", "RUNTIME_ERROR");
//         return;
//
//     case NO_FILE:
//         QMessageBox::critical(this, "Error", "NO_FILE");
//         return;
//
//     case EMPTY_REGION:
//         QMessageBox::critical(this, "Error", "EMPTY_REGION");
//         return;
//
//     case NO_REGION:
//         QMessageBox::critical(this, "Error", "NO_REGION");
//         return;
//
//     case EMPTY_COLLUM:
//         QMessageBox::critical(this, "Error", "EMPTY_COLLUM");
//         return;
//
//     case INVALID_COLLUM:
//         QMessageBox::critical(this, "Error", "INVALID_COLLUM");
//         return;
//
//     case NAME_COLLUM:
//         QMessageBox::critical(this, "Error", "NAME_COLLUM");
//         return;
//
//     case OUTRANGE_COLLUM:
//         QMessageBox::critical(this, "Error", "OUTRANGE_COLLUM");
//         return;
//
//     case TABLE_EMPTY:
//         QMessageBox::critical(this, "Error", "TABLE_EMPTY");
//         return;
//
//     case SUCCESS:
//         break;
//     }
//
//     ui->minTextField->setText(QString::number(ctx->min));
//     ui->maxTextField->setText(QString::number(ctx->max));
//     ui->midTextField->setText(QString::number(ctx->mid));
// }
//
// void MainWindow::clear_labels()
// {
//     ui->errorsLabel->clear();
//     ui->fileNameLabel->setText("No file selected");
// }
