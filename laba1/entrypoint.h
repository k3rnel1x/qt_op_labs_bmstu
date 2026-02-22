#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "appcontext.h"
#include "mainwindow.h"

enum Operation {
    Convert,
    Initialization,
    DeInitialization
};

void doOperation(Operation operation, AppContext* context);


#endif // ENTRYPOINT_H
