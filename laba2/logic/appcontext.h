#ifndef APPCONTEXT_H
#define APPCONTEXT_H
#define COLLUMS_COUNT 7
#include <QProgressDialog>
struct AppContext {
    const char* filename;
    const char* region_filter;

    char*** table;
    size_t table_len;
    char** regions; // all regions without duplicating
    size_t regions_count;

    char** collums; // header collums
    size_t collums_count;

    // calc
    char* calc_region;
    char* calc_collum;
    double min;
    double max;
    double mid;

    double parse_time;

    // TODO hash?
};

#endif // APPCONTEXT_H
