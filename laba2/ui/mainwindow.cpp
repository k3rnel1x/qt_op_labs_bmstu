#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../logic/entrypoint.h"
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
    ctx = (AppContext*)calloc(1, sizeof(AppContext));
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSortingEnabled(true);

    // hide all interfaces
    ui->calcInterface->setVisible(false);
    ui->loadInterface->setVisible(false);

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
    // if (ctx->region_filter)
        // free((char*)ctx->region_filter);

    if (ctx->regions)
        free(ctx->regions);

    // delete collums
    if (ctx->collums)
        free(ctx->collums);
}

void MainWindow::on_openButton_clicked()
{
    // get filename
    QByteArray value = qgetenv("USER");
    // QString filename = QFileDialog::getOpenFileName(this,
    //                                         tr("Open CSV"),
    //                                         "/home/" + value,
    //                                         tr("*.csv"));

    QString filename = QFileDialog::getOpenFileName(this,
                                            tr("Open CSV"),
                                            "/home/" + value + "/Programing/qt-op-labs-bmstu/laba2/test_files",
                                            tr("*.csv"));

    // if leaved return
    if(filename.isEmpty()) {
        if (!ctx->filename)
        {
            ui->openButton->setText("Open your cool file");
            ui->calcInterface->setVisible(false);
            ui->loadInterface->setVisible(false);
        }
        return;
    };


    char* c_str = qstrtoc(filename);
    // insert to context
    const char* old_filename = ctx->filename;
    ctx->filename = c_str;

    // parse_file
    ui->openButton->setText("File is opening..");
    block_ui();
    QApplication::processEvents();

    Result result_code = perform_operation(PARCE_TABLE, ctx);
    if (result_code != SUCCESS) {
        ctx->filename = old_filename;
        ui->openButton->setText(ctx->filename? strrchr(ctx->filename, '/') + 1 : "Open your cool file");
        handle_parce_table_error(result_code);
        unblock_ui();
        return;
    }

    free((char*)old_filename);

    // setup load ui
    ui->openButton->setText(strrchr(ctx->filename, '/') + 1);
    set_available_regions(ctx->regions, ctx->regions_count); // with all item

    // show load ui
    ui->loadInterface->setVisible(true);
    ui->calcInterface->setVisible(false);
    unblock_ui();
}

void MainWindow::on_loadSelectedButton_clicked()
{
    block_ui();
    ctx->region_filter = get_choisen_region_filter();

    ui->tableWidget->clear();
    ui->tableWidget->clearContents();

    if (perform_operation(GET_LOAD_TABLE, ctx) == RUNTIME_ERROR)
    {
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setColumnCount(0);
        QMessageBox::critical(this, "ERROR", "RUNTIME_ERROR");
        return;
    }

    char*** table = ctx->load_table;
    size_t  table_len = ctx->load_table_len;

    ui->tableWidget->setRowCount(table_len-1);
    ui->tableWidget->setColumnCount(COLLUMS_COUNT);

    // set header
    for (size_t c = 0; c < COLLUMS_COUNT; c++)  {
        ui->tableWidget->setHorizontalHeaderItem(c, new QTableWidgetItem( table[0][c] ));
    }


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
            if (progress.wasCanceled()) {
                progress.close();
                ui->tableWidget->clear();
                ui->tableWidget->clearContents();
                ui->tableWidget->setRowCount(0);
                ui->tableWidget->setColumnCount(0);
                ui->tableWidget->update();
                ui->calcInterface->setVisible(false);
                unblock_ui();
                QMessageBox::critical(this, tr("Error"), tr("Table is not loaded"));
                return;
            }
            // TODO used memory overheap(platform specific)
        }
        progress.setValue(r);
        QApplication::processEvents(QEventLoop::DialogExec);
    }

    ui->tableWidget->update();

    progress.close();

    QMessageBox msg;
    msg.setWindowTitle(tr("Table is loaded!"));
    msg.setBaseSize(400, 250);
    msg.setFixedSize(400, 250);
    msg.setText("Load info:");
    msg.setInformativeText(QString("Total lines: %1\nErrors count: %2").arg(ctx->load_table_len-1).arg(ctx->errors_count));
    msg.exec();

    if (ctx->region_filter && strcmp(ctx->region_filter, "All")) {
        ui->loadedRegionField->setText(ctx->region_filter);
        set_calc_regions(&ctx->region_filter, 1);
    } else
    {
        ui->loadedRegionField->setText("All");
        set_calc_regions((const char**)ctx->regions, ctx->regions_count);
    }

    set_calc_collums(ctx->collums, ctx->collums_count, REGION_COLLUM_NUM);

    ui->maxField->clear();
    ui->minField->clear();
    ui->midField->clear();
    ui->calcInterface->setVisible(true);
    unblock_ui();
}

void MainWindow::set_calc_regions(const char** regions, size_t len)
{
    ui->regionList->clear();
    if (!regions) return;

    if (len > 1)
        ui->regionList->addItem("All");

    for (int i = 0; i < len; i++)
        ui->regionList->addItem(regions[i]);
}

void MainWindow::set_available_regions(char** regions, size_t len)
{
    ui->availableRegions->clear();

    if (!regions || len == 0) {
        ui->availableRegions->addItem("Nothing available");
        ui->loadSelectedButton->setVisible(false);
        ui->availableRegions->setEnabled(false);
        return;
    }

    ui->availableRegions->addItem("All");
    for (int i = 0; i < len; ++i)
        ui->availableRegions->addItem(regions[i]);
    ui->loadSelectedButton->setVisible(true);
    ui->availableRegions->setEnabled(true);
}

void MainWindow::set_calc_collums(char** collums, size_t len, size_t region_collum_num)
{
    ui->collumList->clear();
    if (!collums) return;

    for (int i = 0; i < len; i++)
        if (i+1 != region_collum_num)
            ui->collumList->addItem(collums[i]);
}

const char* MainWindow::get_choisen_region_filter()
{
    QString region = ui->availableRegions->currentText();
    char* c_str = qstrtoc(region);
    if (!c_str || !*c_str) return NULL;

    return c_str;
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
    block_ui();
    ui->calcButton->setText("Please wait..");
    QApplication::processEvents();

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

    ui->minField->setText(QString::number(ctx->min));
    ui->maxField->setText(QString::number(ctx->max));
    ui->midField->setText(QString::number(ctx->mid));

    ui->calcButton->setText("Calculate metrix");
    unblock_ui();
}

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

// ############### handlers ###############
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
    // TODO !!
}

