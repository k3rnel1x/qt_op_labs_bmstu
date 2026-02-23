#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "appcontext.h"
#include "mainwindow.h"
#include "errors.h"

enum Operation {
    CONVERT,
    INIT,
    DEINIT,
    CLEAR,
    SWAP,
    SHOWERR
};


Result doOperation(Operation operation, AppContext* ctx);

#endif // ENTRYPOINT_H
