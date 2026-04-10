#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../logic/entrypoint.h"
#include "../logic/appcontext.h"
#include "../config.h"

#include <QHeaderView>
#include <QStandardItemModel>
#include <qmessagebox.h>
#include <QFileDialog>
#include <QProgressDialog>
#include <QMovie>
#include <string.h>
#include <time.h>

#include "../logger.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    clipboard = QApplication::clipboard();
    ui->setupUi(this);
    ctx = (AppContext*)calloc(1, sizeof(AppContext));

#ifdef PRICOLCHICKI
    load_cursor = new QCursor(QPixmap(LOAD_CURSOR_ICON));
    ui->promote->setVisible(true);
    vk_dog = new QMovie(VK_DOG_GIF);
    ui->promoteGifLabel->setVisible(true);
    ui->promoteGifLabel->setEnabled(true);
    ui->promoteGifLabel->setMovie(vk_dog);
    vk_dog->start();
#else
    load_cursor = new QCursor(Qt::WaitCursor);
    ui->promote->setVisible(false);
#endif

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSortingEnabled(true);

    // set cursors
    QCursor point_hand_cursor(Qt::PointingHandCursor);
    ui->calcButton->setCursor(point_hand_cursor);
    ui->copyLoadedLinesCountButton->setCursor(point_hand_cursor);
    ui->copyLoadedRegionButton->setCursor(point_hand_cursor);
    ui->loadSelectedButton->setCursor(point_hand_cursor);
    ui->openButton->setCursor(point_hand_cursor);

    // QCursor i_beam_cursor(Qt::IBeamCursor);
    // ui->loadedRegionField->setCursor(i_beam_cursor);
    // ui->loadedLinesCount->setCursor(i_beam_cursor);
    // ui->maxField->setCursor(i_beam_cursor);
    // ui->minField->setCursor(i_beam_cursor);
    // ui->midField->setCursor(i_beam_cursor);


    connect(ui->regionList, &QComboBox::activated, this, &MainWindow::on_calc_params_changed);
    connect(ui->collumList, &QComboBox::activated, this, &MainWindow::on_calc_params_changed);

    // hide all interfaces
    ui->calcInterface->setVisible(false);
    ui->loadInterface->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete load_cursor;

    delete vk_dog;

    // ### delete context ###
    Params p;
    p.clear_target = OPEN_UI_DATA; perform_operation(CLEAR_CONTEXT, ctx, &p);
    p.clear_target = LOAD_UI_DATA; perform_operation(CLEAR_CONTEXT, ctx, &p);
    p.clear_target = CALC_UI_DATA; perform_operation(CLEAR_CONTEXT, ctx, &p);

    free(ctx);
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
            // ui->calcInterface->setVisible(false);
            // ui->loadInterface->setVisible(false);
        }
        return;
    }

    Params p;
    p.clear_target = OPEN_UI_DATA;
    perform_operation(CLEAR_CONTEXT, ctx, &p);

    char* c_str = qstrtoc(filename);
    // insert to context
    const char* old_filename = ctx->filename;
    ctx->filename = c_str;

    // parse_file
    QApplication::setOverrideCursor(*load_cursor);
    ui->openButton->setText("File is opening..");
    QApplication::processEvents();
    block_ui();


    Result result_code = perform_operation(OPEN_TABLE, ctx, NULL);
    if (result_code != SUCCESS) {
        QApplication::restoreOverrideCursor();
        ctx->filename = old_filename;
        ui->openButton->setText(ctx->filename? strrchr(ctx->filename, '/') + 1 : "Open your cool file");
        handle_parce_table_error(result_code);
        unblock_ui();
        return;
    }
    QApplication::restoreOverrideCursor();

    free((char*)old_filename);

    // setup load ui
    ui->openButton->setText(strrchr(ctx->filename, '/') + 1);
    set_available_regions(ctx->table_all_regions, ctx->regions_count); // with all item
    ui->totalErrorLabel->setText("Total errors: " + QString::number(ctx->errors_count));

    // show load ui
    ui->loadInterface->setVisible(true);
    ui->calcInterface->setVisible(false);
    unblock_ui();
}

void MainWindow::on_loadSelectedButton_clicked()
{
    block_ui();
    // ################################### get load region ######################################
    Params p;
    p.clear_target = LOAD_UI_DATA; perform_operation(CLEAR_CONTEXT, ctx, &p); // clear load data
    ctx->region_to_load = get_load_region();

    ui->tableWidget->clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);

    if (perform_operation(LOAD_TABLE, ctx, NULL) == RUNTIME_ERROR)
    {
        QMessageBox::critical(this, "ERROR", "RUNTIME_ERROR");
        return;
    }

    p.clear_target = CALC_UI_DATA;
    perform_operation(CLEAR_CONTEXT, ctx, &p); // clear calc data

    // ################################# init nessesary stuff #############################

    char*** table = ctx->filtered_table;
    size_t  table_len = ctx->filtered_table_len;
    size_t  collums_count = ctx->collums_count;
    char**  table_header = ctx->table_header;

    ui->tableWidget->setRowCount(table_len);
    ui->tableWidget->setColumnCount(collums_count);

    // ##################################### setup header #####################################
    QApplication::setOverrideCursor(*load_cursor);
    for (size_t c = 0; c < collums_count; c++)  {
        ui->tableWidget->setHorizontalHeaderItem(c, new QTableWidgetItem( table_header[c] ));
    }

    QProgressDialog progress("Table is loading...", "Cancel", 0, table_len, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowFlag(Qt::WindowStaysOnTopHint);
    progress.show();


    // ###################################### setup table #####################################
    for (size_t r = 0; r < table_len; ++r)
    {
        for (size_t c = 0; c < collums_count; ++c)
        {
            ui->tableWidget->setItem(r, c, new QTableWidgetItem( table[r][c] ));
            if (progress.wasCanceled()) {
                progress.close();
                Params p;
                p.clear_target = LOAD_UI_DATA;
                perform_operation(CLEAR_CONTEXT, ctx, &p); // clear load data
                ui->tableWidget->clear();
                ui->tableWidget->clearContents();
                ui->tableWidget->setRowCount(0);
                ui->tableWidget->setColumnCount(0);
                ui->tableWidget->update();
                ui->calcInterface->setVisible(false);
                unblock_ui();
                QApplication::restoreOverrideCursor();
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

    // ###################################### show info ############################################
    const char* region_to_load = ctx->region_to_load;
    char** table_all_regions = ctx->table_all_regions;
    size_t regions_count = ctx->regions_count;

    // set loaded region
    ui->loadedRegionField->setText(region_to_load && strcmp(region_to_load, "All")? region_to_load : "All");
    ui->loadedLinesCount->setText(QString::number(table_len));

    // ###################################### set comboboxes #######################################


    // set calc regions
    if (!region_to_load || !strcmp(region_to_load, "All"))
        set_calc_regions( (const char**)table_all_regions, regions_count );
    else
        set_calc_regions( &region_to_load, 1 );

    // set calc collums
    set_calc_collums( table_header, collums_count, REGION_COLLUM_NUM );

    ui->maxField->clear();
    ui->minField->clear();
    ui->midField->clear();
    ui->calcInterface->setVisible(true);
    QApplication::restoreOverrideCursor();
    unblock_ui();
    QApplication::processEvents();
}

void MainWindow::set_calc_regions(const char** regions, size_t len)
{
    ui->regionList->clear();
    ui->regionList->setEnabled(false);
    if (!regions || !*regions || !len) return;

    if (len == 1)
    {
        ui->regionList->addItem(*regions);
    } else if (len > 1)
    {
        ui->regionList->addItem("All");
        for (int i = 0; i < len; ++i)
            ui->regionList->addItem(regions[i]);
        ui->regionList->setEnabled(true);
    }
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

const char* MainWindow::get_load_region()
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

    return strdup(c_str);
}

void MainWindow::on_calcButton_clicked()
{
    ui->maxField->clear();
    ui->minField->clear();
    ui->midField->clear();
    Params p;
    p.clear_target = CALC_UI_DATA;
    perform_operation(CLEAR_CONTEXT, ctx, &p);

    block_ui();
    QApplication::setOverrideCursor(*load_cursor);
    ui->calcButton->setText("Please wait..");
    QApplication::processEvents();

    QString qregion = ui->regionList->currentText();
    ctx->region_to_calc = qstrtoc(qregion);

    QString qcollum = ui->collumList->currentText();
    ctx->collum_to_calc = qstrtoc(qcollum);

    // ### parce ###
    Result result_code = perform_operation(CALC_METRIX, ctx, NULL);
    if (result_code != SUCCESS) {
        QApplication::restoreOverrideCursor();
        handle_calc_metrix_error(result_code);
        ui->calcButton->setText("Calculate metrix");
        unblock_ui();
        calculated = false;
        return;
    }

    ui->minField->setText(QString::number(ctx->min));
    ui->maxField->setText(QString::number(ctx->max));
    ui->midField->setText(QString::number(ctx->mid));

    ui->calcButton->setText("Calculate metrix");
    QApplication::restoreOverrideCursor();
    unblock_ui();
    QApplication::processEvents();
    calculated = true;
    on_calc_params_changed();
}

void MainWindow::on_copyLoadedRegionButton_clicked()
{
   clipboard->setText(ui->loadedRegionField->toPlainText());
}

void MainWindow::on_copyLoadedLinesCountButton_clicked()
{
    clipboard->setText(ui->loadedLinesCount->toPlainText());
}

void MainWindow::on_calc_params_changed()
{
    if (calculated)
    {
        bool is_enabled = ui->regionList->currentText() == ctx->region_to_calc && ui->collumList->currentText() == ctx->collum_to_calc;
        ui->maxField->setEnabled(is_enabled);
        ui->minField->setEnabled(is_enabled);
        ui->midField->setEnabled(is_enabled);
    }
}

void MainWindow::block_ui()
{
    ui->centralwidget->setEnabled(false);
}

void MainWindow::unblock_ui()
{
    // if (!ui->regionList->isEnabled())
        // ui->regionList->setEnabled(true);
    // if (!ui->collumList->isEnabled())
        // ui->collumList->setEnabled(true);

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
    if (code == SUCCESS) return;
    const char* err_text = NULL;
    switch(code)
    {
        case RUNTIME_ERROR:
            err_text = "RUNTIME_ERROR";
            break;

        case NOT_ENOUGH_INFO:
#ifdef PRICOLCHICKI
            err_text = "Ну нельзя посчитать медиану из двух значений, йоу <a href=https://en.wikipedia.org/wiki/Median>см. wiki<a>";
#else
            err_text = "NOT_ENOUGH_INFO";
#endif
            break;
    }
    QMessageBox::critical(this, "Error", err_text);
}

