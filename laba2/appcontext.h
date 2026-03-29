#ifndef APPCONTEXT_H
#define APPCONTEXT_H
#include <stdio.h>
#include "vector.h"

#define VALID_APPCONTEXT(ctx) ((ctx) && (ctx)->filename && (ctx)->lines.inited == 1 && (ctx)->choised_region && (ctx)->choised_collum < 8)

typedef struct AppContext {
    // mainwindow
    const char*  filename;
    const char*  choised_region;
    char*  choised_collum;

    // logic
    Vector lines;
    size_t errors_count;

    double min;
    double max;
    double mid;
} AppContext;

AppContext* get_ctx();

void del_ctx(AppContext* ctx);

#endif // APPCONTEXT_H
