#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../entrypoint.h"
#include "../logic/appcontext.h"

#define REGION_COLLUM_NUM 2

#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <qmessagebox.h>
#include <QFileDialog>
#include <QProgressDialog>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// TODO delete context method
// TODO pricolchiki

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSortingEnabled(true);
    ctx = (AppContext*)calloc(1, sizeof(AppContext));

    ui->regionList->setEnabled(false);
    ui->collumList->setEnabled(false);
    setAttribute(Qt::WA_TranslucentBackground, false);
    metrix_fields_state(false);

    update_filelabel(NULL);
}

MainWindow::~MainWindow()
{
    delete ui;

    // ### delete context ###
    // delete table
    if (ctx->table) {
        for (size_t i = 0; i < ctx->table_len; i++)
            free(ctx->table[i]);
        free(ctx->table);
    }

    // delete filename
    if (ctx->filename)
        free((char*)ctx->filename);

    // delete regions
    if (ctx->region_to_load)
        free((char*)ctx->region_to_load);

    if (ctx->regions)
        free(ctx->regions);

    // delete collums
    if (ctx->collums)
        free(ctx->collums);
}

void MainWindow::on_openButton_clicked()
{
    // open filedialog in
    QByteArray value = qgetenv("USER");
    QString filename = QFileDialog::getOpenFileName(this,
                                            tr("Open CSV"),
                                            "/home/" + value,
                                            tr("*.csv"));
    if(filename.isEmpty()) return;

    // convert to char*
    char* c_str = qstrtoc(filename);

    // insert to context
    if (ctx->filename)
        free((char*)ctx->filename);
    ctx->filename = c_str;

    // set filelabel
    update_filelabel(ctx->filename);
}

void MainWindow::on_loadButton_clicked()
{
    ctx->region_to_load = get_region_to_load();
    // ### parce ###
    Result result_code = perform_operation(PARCE_TABLE, ctx);
    if (result_code != SUCCESS) {
        handle_parce_table_error(result_code);
        return;
    }

    // ### visualize ##

    // set regions to calc
    set_calc_regions(ctx->regions, ctx->regions_count);
    set_calc_collums(ctx->collums, ctx->collums_count, REGION_COLLUM_NUM);

    // init table
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(COLLUMS_COUNT);
    ui->tableWidget->setRowCount(ctx->table_len-1);

    block_ui();

    char*** table = ctx->table;
    size_t  table_len = ctx->table_len;

    // set header
    for (size_t c = 0; c < COLLUMS_COUNT; c++)
    {
        ui->tableWidget->setHorizontalHeaderItem(c, new QTableWidgetItem( table[0][c] ));
    }

    // visualize progress dialog
    QProgressDialog progress("Table is loading...", "Cancel", 0, table_len-1, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowFlag(Qt::WindowStaysOnTopHint);
    progress.show();

    // set sells
    for (size_t r = 0; r < table_len-1; r++)
    {
        for (size_t c = 0; c < COLLUMS_COUNT; c++)
        {
            ui->tableWidget->setItem(r, c, new QTableWidgetItem( table[r+1][c] ));
            // qDebug() << table[r][c];
            if (progress.wasCanceled()) {
                unblock_ui();
                ui->tableWidget->update();
                return;
            }
            // TODO used memory overheap(platform specific)
        }
        progress.setValue(r);
        QApplication::processEvents(QEventLoop::DialogExec);
    }

    // unblock ui and show table
    unblock_ui();
    ui->tableWidget->update();
}

void MainWindow::update_filelabel(const char* full_filename)
{
    if (!full_filename)
        ui->fileNameLabel->setText("No file selected");
    else
        ui->fileNameLabel->setText(QString(strrchr(ctx->filename, '/') + 1));
}

// void MainWindow::update_window_header()
// {
//
// }

void MainWindow::block_ui()
{
    ui->centralwidget->setEnabled(false);
}

void MainWindow::unblock_ui()
{
    if (!ui->regionList->isEnabled())
        ui->regionList->setEnabled(true);
    if (!ui->collumList->isEnabled())
        ui->collumList->setEnabled(true);

    ui->centralwidget->setEnabled(true);
}

void MainWindow::handle_parce_table_error(Result code)
{
    if (code == SUCCESS) return;
    const char* err_text = NULL;
    switch(code)
    {
        case RUNTIME_ERROR:
            err_text = "RUNTIME_ERROR";
            break;

        case EMPTY_FILE:
            err_text = "EMPTY_FILE";
            break;

        case NO_FILE:
            err_text = "NO_FILE";
            break;

        case INVALID_HEADER:
            err_text = "INVALID_HEADER(PROBABLY MUST BE 7 COLLUMS)";
            break;
    }
    QMessageBox::critical(this, "Error", err_text);
}

void MainWindow::handle_calc_metrix_error(Result code)
{
}

const char* MainWindow::get_region_to_load()
{
    QString region = ui->loadRegionField->toPlainText();
    char* c_str = qstrtoc(region);
    if (!c_str || !*c_str) return NULL;

    return c_str;
}

void MainWindow::metrix_fields_state(bool enabled)
{
    ui->maxField->setEnabled(enabled);
    ui->minField->setEnabled(enabled);
    ui->midField->setEnabled(enabled);
}


void MainWindow::set_calc_regions(char** regions, size_t len)
{
    ui->regionList->clear();
    if (!regions) return;

    for (int i = 0; i < len; i++)
        ui->regionList->addItem(regions[i]);
}

void MainWindow::set_calc_collums(char** collums, size_t len, size_t region_collum_num)
{
    ui->collumList->clear();
    if (!collums) return;

    for (int i = 0; i < len; i++)
        if (i+1 != region_collum_num)
            ui->collumList->addItem(collums[i]);
}

char* MainWindow::qstrtoc(QString& qstr)
{
    if (qstr.isEmpty()) return NULL;

    QByteArray bytearray = qstr.toUtf8();
    const char* c_str = bytearray.constData();

    char* mem = (char*)calloc(qstr.length() + 1, sizeof(char));
    strcpy(mem, c_str);
    return mem;
}

void MainWindow::on_calcButton_clicked()
{
    QString qregion = ui->regionList->currentText();
    ctx->calc_region = qstrtoc(qregion);

    QString qcollum = ui->collumList->currentText();
    ctx->calc_collum = qstrtoc(qcollum);

    // ### parce ###
    Result result_code = perform_operation(CALC_METRIX, ctx);
    if (result_code != SUCCESS) {
        handle_calc_metrix_error(result_code);
        return;
    }

    // ui->minTextField->setText(QString::number(ctx->min));
    // ui->maxTextField->setText(QString::number(ctx->max));
    // ui->midTextField->setText(QString::number(ctx->mid));
}
//
// void MainWindow::clear_labels()
// {
//     ui->errorsLabel->clear();
//     ui->fileNameLabel->setText("No file selected");
// }
