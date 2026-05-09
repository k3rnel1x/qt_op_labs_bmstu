#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "appcontext.h"
#include <entrypoint.h>
#include "ui_mainwindow.h"
#include "surfacedrawer/surfacedrawer.h"

#define DEFAULTLOADTEXT "Load Data"

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

protected:
    // void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_loadDataButton_clicked();
    void on_renderButton_clicked();
    void on_stepSlider_valueChanged();
    void on_spinboxes_valueChanged();

private:

    void handleResult(ResultCode result);

    void updateConfigureUI();

    char* qstrtoc(const QString& qstr);

    AppContext* context;
    Ui::MainWindow* ui;
    SurfaceDrawer drawer;
};
#endif // MAINWINDOW_H
