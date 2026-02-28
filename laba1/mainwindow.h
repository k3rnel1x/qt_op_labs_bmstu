#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QButtonGroup>
#include <QClipboard>

// #include <iostream>
#include <qabstractbutton.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qstringview.h>
#include <string.h>

#include "appcontext.h"
#include "entrypoint.h"
#include "errors.h"

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
    QClipboard*     clipboard;

    // Init
    void initRadioButtons();

    // Getters
    void getNumSystems(AppContext* context);
    void getInputText(AppContext* context);
    void getCustomOutputSystem(AppContext* context);
    void getCustomInputSystem(AppContext* context);

    // Setters
    void setOutputText(AppContext* context);
    void setInputText(AppContext* context);
    void setInputSystemText(AppContext* context);
    void setOutputSystemText(AppContext* context);

    // Utils
    void handleResult(Result res);
};
#endif // MAINWINDOW_H
