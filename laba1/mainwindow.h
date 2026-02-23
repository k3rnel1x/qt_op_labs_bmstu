#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QButtonGroup>

// #include <iostream>
#include <qabstractbutton.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qstringview.h>
#include <string.h>

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

    void on_radioButtonCustomSystemInput_clicked();

    void on_radioButtonCustomSystemOutput_clicked();

private:
    Ui::MainWindow* ui;
    AppContext*     ctx;
    QButtonGroup*   inputRadioButtons;
    QButtonGroup*   outputRadioButtons;

    // Init
    void groupRadioButtons();

    // Getters
    void getNumSystems(AppContext* context);
    char* getInputText();
    char* getCustomOutputSystem();
    char* getCustomInputSystem();

    // Setters
    void setOutText(AppContext* context);
};
#endif // MAINWINDOW_H
