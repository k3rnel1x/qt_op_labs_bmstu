#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "appcontext.h"
#include "mainwindow.h"

enum Operation {
    CONVERT,
    INIT,
    DEINIT,
    CLEAR
};

void doOperation(Operation operation, AppContext* ctx);


#endif // ENTRYPOINT_H
