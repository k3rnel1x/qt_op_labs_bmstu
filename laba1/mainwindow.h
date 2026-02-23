#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QButtonGroup>
#include "appcontext.h"
#include "entrypoint.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_convertButton_clicked();

    void on_swapNumSystemButton_clicked();

    void on_CopyToClipboardLeftButton_clicked();

    void on_CopyToClipboardRightButton_clicked();

private:
    Ui::MainWindow* ui;
    AppContext*     ctx;
    QButtonGroup*   inputRadioButtons;
    QButtonGroup*   outputRadioButtons;

    // Init
    void groupRadioButtons();

    // Getters
    const char* getInputText();
    void getNumSystems(AppContext* context);

    // Setters
    void setOutputText(AppContext* context);
};
#endif // MAINWINDOW_H
