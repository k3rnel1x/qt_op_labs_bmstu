#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "appcontext.h"
#include <entrypoint.h>
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    // class State {

    // };

    // class FileSelectioning : public State {
    //     void selectFile();
    // };

    // class ConfiguringRender : public FileSelectioning {
    //     void updateStep();
    //     void updateRange();
    // };

    // class Rendering : public ConfiguringRender {
    //     void render();
    // };

    // typedef enum StateType {
    //     FileSelectioning,
    //     ConfiguringRender,
    //     Rendering
    // } StateType;

    // class StatesManager {
    // public:
    //     void updateState(State& targetState, Params* params);

    // private:
    //     State currState;
    // };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadDataButton_clicked();
    void on_renderButton_clicked();
    void on_stepSlider_valueChanged();

private:

    void handleResult(ResultCode result);

    void updateConfigureUI();

    char* qstrtoc(const QString& qstr);

    AppContext* context;
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
