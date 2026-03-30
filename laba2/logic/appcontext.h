#ifndef APPCONTEXT_H
#define APPCONTEXT_H
#define COLLUMS_COUNT 7

struct AppContext {
    const char*  filename;

    char*** table;
    size_t table_len;
    const char** regions; // all regions without duplicating
    size_t regions_count;

    size_t curr_region_index;
    size_t curr_collum_number;

    double min;
    double max;
    double mid;

    double load_time;
};

#endif // APPCONTEXT_H
