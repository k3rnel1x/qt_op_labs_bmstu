#ifndef APPCONTEXT_H
#define APPCONTEXT_H
#include <stdio.h>
#include "vector.h"

#define VALID_APPCONTEXT(ctx) ((ctx) && (ctx)->opened_file && (ctx)->lines.inited == 1 && (ctx)->choised_region && (ctx)->choised_collum < 8)

typedef struct AppContext {
    // mainwindow
    FILE*  opened_file;
    char*  choised_region;
    size_t choised_collum;

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
