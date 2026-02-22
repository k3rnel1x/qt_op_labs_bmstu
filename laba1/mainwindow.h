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

private:
    Ui::MainWindow *ui;
    AppContext* ctx;
    QButtonGroup* inputRadioButtons;
    QButtonGroup* outputRadioButtons;

    void onPushConvertButton();
    void updateOutText();
    void getInputText();
	void initRadioButtons();
    void UpdateLSystem();
    void UpdateRSystem();
};
#endif // MAINWINDOW_H
