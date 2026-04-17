#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../logic/entrypoint.h"
#include "../logic/appcontext.h"
#include "../config.h"
#include "graph.h"

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
    if (!ctx) throw std::bad_alloc();

#ifdef PRICOLCHICKI
    load_cursor = new QCursor(QPixmap(LOAD_CURSOR_ICON));
    ui->promote->setVisible(true);
    vk_dog = new QMovie(VK_DOG_GIF);
    ui->promoteGifLabel->setVisible(true);
    ui->promoteGifLabel->setEnabled(true);
    ui->promoteGifLabel->setMovie(vk_dog);
    vk_dog->start();
    this->setWindowTitle("Ultrafast mega thin and functional increrable pedantic amazing creeper 2014 melancholic supervised AI ChatGeminiSeek pro SALE 50% nice cute strong cool table viewer and calculate metriX application in qt");
#else
    load_cursor = new QCursor(Qt::WaitCursor);
    ui->promote->setVisible(false);
    this->setWindowTitle("laba2");
#endif
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSortingEnabled(true);

    // set cursors
    QCursor point_hand_cursor(Qt::PointingHandCursor);
    ui->calcButton->setCursor(point_hand_cursor);
    ui->copyLoadedLinesCountButton->setCursor(point_hand_cursor);
    ui->copyLoadedRegionButton->setCursor(point_hand_cursor);
    ui->loadSelectedButton->setCursor(point_hand_cursor);
    ui->visualizateButton->setCursor(point_hand_cursor);
    ui->openButton->setCursor(point_hand_cursor);

    connect(ui->regionList, &QComboBox::activated, this, &MainWindow::on_calc_params_changed);
    connect(ui->collumList, &QComboBox::activated, this, &MainWindow::on_calc_params_changed);

    // hide all interfaces
#ifndef ENABLE_GRAPH
    ui->visualizateButton->setVisible(false);
#endif
    ui->calcInterface->setVisible(false);
    ui->loadInterface->setVisible(false);
    ui->tabWidget->setCurrentIndex(0);
    ui->tab_graph_widget->setEnabled(false);
    ui->tab_table_widget->setEnabled(false);
    ui->tabWidget->setTabVisible(1, false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete load_cursor;
#ifdef PRICOLCHICKI
    delete vk_dog;
#endif
    // ### delete context ###
    Params p;
    p.clear_target = OPEN_UI_DATA; perform_operation(ERASE_CONTEXT, ctx, &p);
    p.clear_target = LOAD_UI_DATA; perform_operation(ERASE_CONTEXT, ctx, &p);
    p.clear_target = CALC_UI_DATA; perform_operation(ERASE_CONTEXT, ctx, &p);

    free(ctx);
}

void MainWindow::on_openButton_clicked()
{
    // get filename
    QByteArray username = qgetenv("USER");
    // QString filename = QFileDialog::getOpenFileName(this,
    //                                         tr("Open CSV"),
    //                                         "/home/" + value,
    //                                         tr("*.csv"));
    // QString filename = QFileDialog::getOpenFileName(this,
    //                                         tr("Open CSV"),
    //                                         "/home/" + value + "/Programing/qt-op-labs-bmstu/laba2/test_files",
    //                                         tr("*.csv"),
    //                                                 );

    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Table (*.csv)"));
#ifdef DEBUG
    dialog.setDirectory("/home/" + username + "/Programing/qt-op-labs-bmstu/laba2/test_files");
#endif
    // dialog.setFileMode(QFileDialog::ReadOnly);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.exec();

    if(dialog.selectedFiles().isEmpty()) {
        if (!ctx->filename)
        {
            ui->openButton->setText("Open your cool file");
            // ui->calcInterface->setVisible(false);
            // ui->loadInterface->setVisible(false);
        }
        return;
    }

    QString qfilename = dialog.selectedFiles().at(0);

    // parse_file
    QApplication::setOverrideCursor(*load_cursor);
    ui->openButton->setText("File is opening..");
    block_ui();
    QApplication::processEvents();

    // insert to params
    Params p = {0};
    // p.clear_target = OPEN_UI_DATA;
    // perform_operation(CLEAR_CONTEXT, ctx, &p);

    // execute operation
    p.filename = qstrtoc(qfilename);
    Result result_code = perform_operation(OPEN_TABLE, ctx, &p);
    if (result_code != SUCCESS) {
        QApplication::restoreOverrideCursor();
        free((char*)p.filename);
        if (ctx->filename)
            ui->openButton->setText(strrchr(ctx->filename, '/') + 1);
        else
            ui->openButton->setText("Open your cool file");
        handle_parce_table_error(result_code);
        unblock_ui();
        return;
    }
    QApplication::restoreOverrideCursor();

    ui->openButton->setText(strrchr(ctx->filename, '/') + 1);

    // setup load ui
    ui->openButton->setText(strrchr(ctx->filename, '/') + 1);
    set_available_regions(ctx->table_all_regions, ctx->regions_count); // with all item
    ui->totalErrorLabel->setText("Total errors: " + QString::number(ctx->errors_count));

    // show load ui
    ui->loadInterface->setVisible(true);
    ui->calcInterface->setVisible(false);
    ui->tabWidget->setTabVisible(1, false);
    unblock_ui();
}

void MainWindow::on_loadSelectedButton_clicked()
{
    block_ui();
    // ################################### Prepare params ######################################
    Params p;
    p.region_to_load = get_load_region();
    ui->tableWidget->clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);

    if (perform_operation(LOAD_TABLE, ctx, &p) != SUCCESS)
    {
        QMessageBox::critical(this, "ERROR", "RUNTIME_ERROR");
        return;
    }
    // ################################# init nessesary stuff #############################

    const char*** table     = (const char***)ctx->filtered_table;
    const size_t  table_len = ctx->filtered_table_len;

    const char**  table_header  = (const char**)ctx->table_header;
    const size_t  collums_count = ctx->collums_count;

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
                ui->tableWidget->clear();
                ui->tableWidget->clearContents();
                ui->tableWidget->setRowCount(0);
                ui->tableWidget->setColumnCount(0);
                ui->tableWidget->update();
                ui->tab_table_widget->setEnabled(false);
                ui->tabWidget->setTabVisible(1, false);
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
    ui->tab_table_widget->setEnabled(true);
    ui->visualizateButton->setEnabled(false);
    QApplication::restoreOverrideCursor();
    ui->tabWidget->setTabVisible(1, false);
    unblock_ui();
    QApplication::processEvents();
}

void MainWindow::on_visualizateButton_clicked()
{
    if (!graph){
        graph = new MetrixGraph;
        graph->setParent(ui->tab_graph_widget);
        ui->tab_graph_widget->layout()->addWidget(graph);
    }

    if( perform_operation(CALC_VISUALIZATION, ctx, NULL) != SUCCESS)
    {
        QMessageBox::critical(this, "ERROR", "VISUALIZE ERROR");
        return;
    }

    graph->update_data((const char***)ctx->year_sorted_table, ctx->year_sorted_table_len, ctx->calculated_collum_idx);
    // qDebug() << ctx->year_sorted_table << year_so;
    ui->tabWidget->setTabVisible(1, true);
    ui->tab_graph_widget->setEnabled(true);
    ui->tabWidget->setCurrentIndex(1);
    graph->show();
}

void MainWindow::on_calcButton_clicked()
{
    ui->maxField->clear();
    ui->minField->clear();
    ui->midField->clear();

    block_ui();
    QApplication::setOverrideCursor(*load_cursor);
    ui->calcButton->setText("Please wait..");
    QApplication::processEvents();

    Params p;
    QString qregion = ui->regionList->currentText();
    p.region_to_calc = qstrtoc(qregion);

    QString qcollum = ui->collumList->currentText();
    p.collum_to_calc = qstrtoc(qcollum);

    // ### parce ###
    Result result_code = perform_operation(CALC_METRIX, ctx, &p);
    if (result_code != SUCCESS) {
        QApplication::restoreOverrideCursor();
        handle_calc_metrix_error(result_code);
        ui->calcButton->setText("Calculate metrix");
        unblock_ui();
        is_calculated = false;
        return;
    }

    ui->minField->setText(QString::number(ctx->min));
    ui->maxField->setText(QString::number(ctx->max));
    ui->midField->setText(QString::number(ctx->mid));

    ui->calcButton->setText("Calculate metrix");
    QApplication::restoreOverrideCursor();
    unblock_ui();
    QApplication::processEvents();
    is_calculated = true;
#ifdef ENABLE_GRAPH
    ui->tabWidget->setTabVisible(1, false);
    ui->visualizateButton->setEnabled(true);
#endif
    ui->minField->setEnabled(true);
    ui->midField->setEnabled(true);
    ui->maxField->setEnabled(true);
    // on_calc_params_changed();
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

void MainWindow::set_calc_collums(const char** collums, size_t len, size_t region_collum_num)
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
    if (is_calculated)
    {
        bool is_enabled = (ctx->calculated_region  == ui->regionList->currentText()) && ( ctx->calculated_collum == ui->collumList->currentText());
        ui->maxField->setEnabled(is_enabled);
        ui->minField->setEnabled(is_enabled);
        ui->midField->setEnabled(is_enabled);
#ifdef ENABLE_GRAPH
        ui->visualizateButton->setEnabled(is_enabled);
        ui->tabWidget->setTabVisible(1, is_enabled);
#endif
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

