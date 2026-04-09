// TODO split this file

#include <cinttypes>
#include <qlogging.h>
#include <QDebug>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "appcontext.h"
#include "entrypoint.h"
#include "result.h"
#include "array.h"

#define STR_SIZE 200
#define SEP ','

// #define TABLE_BASE_LENGHT   100
// #define TABLE_LENGHT_SCALE 2

// #define REGIONS_BASE_LENGHT 100
// #define REGIONS_LENGHT_SCALE 2
#define REGION_COLLUM_NUM 2
#define WEIGHT_COLLUM_NUM 6

int valid_year(char* year)
{
    while (*year)
        if (!('0' <= *year && *year <= '9'))
            return 0;
        else
            year++;

    return 1;
}

// incapsulated in logic.cpp
int set_pointers(char** ptrs, char* str)
{
    size_t i = 0;
    for (char* ptr = str; *ptr;)
    {
        char* print_ptr = ptr;

        while (*ptr && *ptr != SEP && *ptr != '\n') ptr++;

        if (*ptr) {
            *ptr = 0;
            ptr++;
        }

        if (!*print_ptr || print_ptr == ptr)
            return 0;

        ptrs[i++] = print_ptr;
    }

    return 1;
}

void clear_pointers(AppContext* ctx)
{
    if (!ctx) return;


    // clear table pointer
    if (ctx->table) {
        for (size_t i = 0; i < ctx->table_len; i++)
            free(ctx->table[i]);
        free(ctx->table);
    }
    ctx->table = NULL;

    // clear regions pointer
    if (ctx->regions)
        free(ctx->regions);
    ctx->regions = NULL;

    // clear collums pointer
    if (ctx->collums)
        free(ctx->collums);
    ctx->collums = NULL;
}

size_t count_collums(char* table_header)
{
    if (!table_header || *table_header == SEP || *(strrchr(table_header, SEP) + 1) == 0) return 0;

    size_t count = 0;
    char* ptr;
    while (ptr = strchr(table_header, SEP))
    {
        if (*ptr != *(ptr-1))
            ++count;
    };

    return count;
}

Result parse_table(AppContext* ctx)
{
    // ######## checks ########
    if (!ctx) return RUNTIME_ERROR;

    FILE *f = fopen(ctx->filename, "r");
    if (!f) return NO_FILE;
    if (feof(f) || getc(f) == EOF) return EMPTY_FILE;
    rewind(f);

    // handle header (ctx->collums_count, ctx->table_header)
    size_t parsed_raw_size = STR_SIZE;
    {
        // get collums count
        char* table_header = (char*)calloc(STR_SIZE, sizeof(char));
        if (!table_header) return RUNTIME_ERROR;
        fgets(table_header, STR_SIZE, f);
        *strrchr(table_header, '\n') = 0;

        ctx->collums_count = count_collums(table_header);
        if (!ctx->collums_count) { free(table_header); return INVALID_HEADER; }
        parsed_raw_size = ctx->collums_count * sizeof(char*) + STR_SIZE;

        // parse header
        char** raw = (char**)calloc(parsed_raw_size, sizeof(char));
        char*  str = (char*)raw + ctx->collums_count;

        if (!fgets(str, STR_SIZE, f)) {
            free(raw);
            return RUNTIME_ERROR;
        }

        set_pointers(raw, str); // no need checks
        ctx->table_header = raw;
    }

    // ######## init ########
    // init arrays
    Array* table_content = get_array(); // char**
    if (!table_content) return RUNTIME_ERROR;
    Array* table_regions = get_array(); // char*
    if (!table_regions) return RUNTIME_ERROR;


    // ######## start parsing ########
    clock_t start_time = clock();

    // parse all other
    ctx->errors_count = 0;
    do{
        // alloc new line
        char** raw = (char**)calloc(parsed_raw_size, sizeof(char));
        char*  str = (char*)(raw + COLLUMS_COUNT);

        // fill str
        if (!fgets(str, STR_SIZE, f)) {
            free(raw);
            break;
        }

        // parse str
        if ( !set_pointers(raw, str) || !valid_year(raw[0]) )  {
            free(raw);
            ++ctx->errors_count;
            continue; // skip line if invalid
        }

        // put to table
        push(table_content, raw);

        // put region if new
        int new_region = 1;
        for (size_t i = 0; new_region && i < regions->count; ++i)
            new_region = strcmp(  getp(table_regions, i, char*), raw[REGION_COLLUM_NUM-1] );

        if (new_region)
            push(regions, raw[REGION_COLLUM_NUM-1]);
    } while (!feof(f));

    // stop timer //
    clock_t end_time = clock();
    double parse_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    if (regions->count == 0)
        delete_arr(&regions);

    // ### clear ###
    clear_pointers(ctx);

    // ### push results to context ###
    ctx->table = (char***)table->data;
    ctx->table_len = table->count;
    ctx->parse_time = parse_time;

    ctx->regions = regions? (char**)regions->data : NULL;
    ctx->regions_count = regions? regions->count : 0;

    ctx->collums = (char**)collums->data;
    ctx->collums_count = COLLUMS_COUNT;
    return SUCCESS;
}

Result get_load_table(AppContext* ctx)
{
    if (!ctx) return RUNTIME_ERROR;

    if (!ctx->region_filter || !strcmp(ctx->region_filter, "All"))
    {
        ctx->load_table = ctx->table;
        ctx->load_table_len = ctx->table_len;
        return SUCCESS;
    }

    Array* load_table = get_array();
    if (!load_table) return RUNTIME_ERROR;

    push(load_table, ctx->table[0]);

    for (int i = 0; i < ctx->table_len; ++i) {
        if (strcmp(ctx->table[i][REGION_COLLUM_NUM-1], ctx->region_filter) == 0) {
            push(load_table, ctx->table[i]);
        }
    }

    if (load_table->count == 0) {
        free(load_table);
        ctx->load_table = NULL;
        ctx->load_table_len = 0;
        return SUCCESS;
    }

    // insert to context
    ctx->load_table = (char***)load_table->data;
    ctx->load_table_len = load_table->count;
    return SUCCESS;
}

// TODO speedup this shit
Result calc_metrix(AppContext* ctx)
{
    if (!ctx || !ctx->load_table || !ctx->load_table_len || ctx->load_table_len == 1) return RUNTIME_ERROR;
    if (!ctx->calc_collum) return RUNTIME_ERROR;

    // get target collum_idx
    size_t collum_idx = -1;
    for (size_t i = 0; i < COLLUMS_COUNT; ++i)
        if (!strcmp(ctx->load_table[0][i], ctx->calc_collum)) {
            collum_idx = i;
            break;
        }
    if (collum_idx == -1) return RUNTIME_ERROR;

    // handle special cases
    if (ctx->load_table_len == 2)      // 1 elem = error, wiki: https://en.wikipedia.org/wiki/Median
        return NOT_ENOUGH_INFO;

    if (ctx->load_table_len == 3)
    {
        double min = atof(ctx->load_table[0][collum_idx]);
        double max = atof(ctx->load_table[0][collum_idx]);
        if (min > max) {
            double tmp = min;
            min = max;
            max = tmp;
        }
        ctx->max = max;
        ctx->min = min;
        ctx->mid = (max + min) / 2.0;
        return SUCCESS;
    } // 2 elems

    // handle other cases

    // qDebug("Count: %lu, Calculated capacity: %lu\n", ctx->load_table_len, ARR_INIT_SIZE * (size_t)pow(2, (int)ceil(log2(ctx->load_table_len / ARR_INIT_SIZE))));

    // fill sorted table
    Array* sorted_table = get_array();
    if (!sorted_table) return RUNTIME_ERROR;
    // peek only calc region
    if (!ctx->calc_region || !strcmp(ctx->calc_region, "All"))
    {
        for (size_t i = 1; i < ctx->load_table_len; i++)
            push(sorted_table, ctx->load_table[i][collum_idx]);

    } else {
        for (size_t i = 1; i < ctx->load_table_len; i++)
        {
            if (!strcmp(ctx->load_table[i][REGION_COLLUM_NUM-1], ctx->calc_region))
            {
                push(sorted_table, ctx->load_table[i][collum_idx]);
            }
        }
    }

    // sort table
    for (size_t i = 0; i < sorted_table->count - 1; ++i)
    {
        for (size_t j = i + 1; j < sorted_table->count; ++j)
        {
            if (atof((char*)sorted_table->data[i]) > atof((char*)sorted_table->data[j]))
            {
                void* tmp = sorted_table->data[i];
                sorted_table->data[i] = sorted_table->data[j];
                sorted_table->data[j] = tmp;
            }
        }
    }

    // insert to context
    double min = atof((char*)sorted_table->data[0]);
    double max = atof((char*)sorted_table->data[sorted_table->count - 1]);

    double mid;
    if (sorted_table->count % 2 == 0)
    {
        double l = atof((char*)sorted_table->data[sorted_table->count/2]);
        double r = atof((char*)sorted_table->data[sorted_table->count/2-1]);
        mid = (l + r) / 2.0;
    } else {
        mid = atof((char*)sorted_table->data[sorted_table->count/2]);
    }

    ctx->max = max;
    ctx->min = min;
    ctx->mid = mid;

    // for (size_t i = 0; i < sorted_table->count; ++i){
    //     qDebug("Region: |%s|\n", get_charpp(sorted_table,i)[1]);
    // }

    return SUCCESS;
}


// TODO implement calc_metrix with all cases!!