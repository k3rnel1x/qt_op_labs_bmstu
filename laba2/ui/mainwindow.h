#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QHeaderView>
#include <QStandardItemModel>

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
    void on_loadButton_clicked();
    void on_openButton_clicked();
    void on_calcButton_clicked();

private:

    // void update_metrix(double max, double min);
    // void update_filelabel(const char* full_filename);
    // void update_window_header();
    void block_ui();
    void unblock_ui();
    void handle_parce_table_error(Result code);
    void handle_calc_metrix_error(Result code);
    const char* get_region_filter();
    void metrix_fields_state(bool enabled);
    void set_available_regions(char** regions, size_t len);
    void set_calc_regions(char** regions, size_t len);
    void set_calc_collums(char** collums, size_t len, size_t region_collum_num);

    char* qstrtoc(QString& qstr);
    AppContext* ctx;
    QTableWidget* table_view;
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
