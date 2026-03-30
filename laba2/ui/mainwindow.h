#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QHeaderView>
#include <QStandardItemModel>

#include "../logic/appcontext.h"

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
    // void on_calcButton_clicked();

private:

    // void update_metrix(double max, double min);
    void update_filelabel();
    void update_window_header();
    // void update_regions();
    // void update_collums();
    AppContext* ctx = nullptr;
    QTableWidget* table_view = nullptr;
    Ui::MainWindow* ui = nullptr;
};
#endif // MAINWINDOW_H
