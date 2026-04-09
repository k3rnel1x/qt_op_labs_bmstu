#ifndef APPCONTEXT_H
#define APPCONTEXT_H
#define COLLUMS_COUNT 7

#include <cstddef>

struct AppContext {

    // Open UI data
    const char* filename;

    char**  table_header;
    size_t  collums_count;

    char*** table_content;
    size_t  table_content_len;

    char** table_all_regions;
    size_t table_all_regions_len;

    // Load UI data
    const char* region_to_load;

    char*** filtered_table_file;
    size_t  filtered_table_len;

    // Calc UI data
    char* region_to_calc;
    char* collum_to_calc;
    double min;
    double max;
    double mid;

    // other data
    double parse_time;
    size_t errors_count;

    // TODO hash?
};

#endif // APPCONTEXT_H
