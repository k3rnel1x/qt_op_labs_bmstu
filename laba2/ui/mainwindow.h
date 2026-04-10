#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QClipboard>

#include "../config.h"
#include "../logic/appcontext.h"
#include "../logic/result.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Open UI
    void on_openButton_clicked();

    // Load UI
    void on_loadSelectedButton_clicked();

    // Calc UI
    void on_calcButton_clicked();
    void on_copyLoadedRegionButton_clicked();
    void on_copyLoadedLinesCountButton_clicked();

    // utils for ui
    void on_calc_params_changed();

private:
    bool calculated = false; // is calc was already performed

    // Load UI
    void set_available_regions(char** regions, size_t len);

    // Calc UI
    void set_calc_regions(const char** regions, size_t len);
    void set_calc_collums(char** collums, size_t len, size_t region_collum_num);

    // ui utils
    void block_ui();
    void unblock_ui();

    // errors handlers
    void handle_parce_table_error(Result code);
    void handle_calc_metrix_error(Result code);

    // data utils
    const char* get_load_region();
    char* qstrtoc(QString& qstr);

#ifdef PRICOLCHICKI
    QMovie* vk_dog;
#endif


    AppContext* ctx;
    QTableWidget* table_view;
    Ui::MainWindow* ui;
    QClipboard* clipboard;
    QCursor* load_cursor;
};
#endif // MAINWINDOW_H
