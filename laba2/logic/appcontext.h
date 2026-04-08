#ifndef APPCONTEXT_H
#define APPCONTEXT_H
#define COLLUMS_COUNT 7

#include <cstddef>

struct AppContext {
    const char* filename;
    const char* region_filter;

    char*** table;
    size_t  table_len;
    char*** load_table; // loaded regions
    size_t  load_table_len;

    char** regions; // all regions
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
    size_t errors_count;

    // TODO hash?
};

#endif // APPCONTEXT_H
