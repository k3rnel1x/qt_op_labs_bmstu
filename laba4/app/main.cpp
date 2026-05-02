#include "../gui/mainwindow.h"
#include <QApplication>
#include "logger.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Logger::get_instance().logDebug("Application started");
    return a.exec();
}
