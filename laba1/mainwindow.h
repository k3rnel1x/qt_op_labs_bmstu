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
    void on_pushButton_clicked();
    void on_CopyToClipboardLeftButton_clicked();
    void on_CopyToClipboardRightButton_clicked();

private:
    Ui::MainWindow *ui;
    AppContext* ctx;
    QButtonGroup* inputRadioButtons;
    QButtonGroup* outputRadioButtons;

    // init
    void initRadioButtons();

    // getters
    const char* getInputText();
    void getNumSystems();

    // setters
    void setOutputText();
    void setRightSystem();
    void setLeftSystem();
};
#endif // MAINWINDOW_H
