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

size_t count_collums(char* table_header)
{
    if (!table_header || *table_header == SEP || *(strrchr(table_header, SEP) + 1) == 0) return 0;
    size_t count = 0;

    for (char* ptr = table_header; *ptr; ptr++) {
        if (*ptr == SEP)
            ++count;
    }

    return ++count;
}

Result open_table(AppContext* ctx)
{
    // ######## checks ########
    if (!ctx || !ctx->filename) return RUNTIME_ERROR;

    FILE *f = fopen(ctx->filename, "r");
    if (!f) return NO_FILE;
    if (feof(f) || getc(f) == EOF) return EMPTY_FILE;
    rewind(f);

    // handle header
    size_t parsed_raw_size = STR_SIZE;
    size_t collums_count = 0;
    char** table_header;
    {
        // get collums count
        char* line = (char*)calloc(STR_SIZE, sizeof(char));
        if (!line) return RUNTIME_ERROR;
        fgets(line, STR_SIZE, f);
        fseek(f, 0, SEEK_SET);

        collums_count = count_collums(line);
        free(line);
        if (!collums_count) return INVALID_HEADER;
        parsed_raw_size = collums_count * sizeof(char*) + STR_SIZE;

        // parse header
        char** raw = (char**)calloc(parsed_raw_size, sizeof(char));
        char*  str = (char*)(raw + collums_count);

        if (!fgets(str, STR_SIZE, f))
        {
            free(raw);
            return RUNTIME_ERROR;
        }

        if (!set_pointers(raw, str)) {
            free(raw);
            return INVALID_HEADER;
        }

        table_header = raw;
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
    size_t errors_count = 0;
    do{
        // alloc new line
        char** raw = (char**)calloc(parsed_raw_size, sizeof(char));
        char*  str = (char*)(raw + collums_count);

        // fill str
        if (!fgets(str, STR_SIZE, f)) {
            free(raw);
            break;
        }

        // parse str
        if ( !set_pointers(raw, str) || !valid_year(raw[0]) )  {
            free(raw);
            ++errors_count;
            continue; // skip line if invalid
        }

        // put to table
        if (!push(table_content, raw)) {
            delete_arr(&table_content);
            delete_arr(&table_regions);
            free(raw);
            return RUNTIME_ERROR;
        };

        // put region if new
        int new_region = 1;
        for (size_t i = 0; new_region && i < table_regions->count; ++i)
            new_region = strcmp(  getp(table_regions, i, char*), raw[REGION_COLLUM_NUM-1] );

        if (new_region)
        {
            if (!push(table_regions, raw[REGION_COLLUM_NUM-1]))
            {
                delete_arr(&table_content);
                delete_arr(&table_regions);
                free(raw);
                return RUNTIME_ERROR;
            }
        }
    } while (!feof(f));

    // stop timer //
    clock_t end_time = clock();
    double parse_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    if (table_regions->count == 0)
        delete_arr(&table_regions);



    // ### push results to context ###
    ctx->table_header  = table_header;
    ctx->collums_count = collums_count;

    ctx->table_content = (char***)table_content->data;
    ctx->table_content_len = table_content->count;

    ctx->table_all_regions = (char**)table_regions->data;
    ctx->regions_count = table_regions->count;

    ctx->parse_time = parse_time;

    return SUCCESS;
}

Result load_table(AppContext* ctx)
{
    if (!ctx) return RUNTIME_ERROR;

    if (!ctx->region_to_load || !strcmp(ctx->region_to_load, "All"))
    {
        ctx->filtered_table = ctx->table_content;
        ctx->filtered_table_len = ctx->table_content_len;
        return SUCCESS;
    }

    Array* load_table = get_array();
    if (!load_table) return RUNTIME_ERROR;

    for (int i = 0; i < ctx->table_content_len; ++i) {
        if (strcmp(ctx->table_content[i][REGION_COLLUM_NUM-1], ctx->region_to_load) == 0) {
            push(load_table, ctx->table_content[i]);
        }
    }

    if (load_table->count == 0) {
        delete_arr(&load_table);
        ctx->filtered_table = NULL;
        ctx->filtered_table_len = 0;
        return SUCCESS;
    }

    // insert to context
    ctx->filtered_table = (char***)load_table->data;
    ctx->filtered_table_len = load_table->count;
    return SUCCESS;
}

// TODO speedup this shit
Result calc_metrix(AppContext* ctx)
{
    // if (!ctx || !ctx->load_table || !ctx->load_table_len || ctx->load_table_len == 1) return RUNTIME_ERROR;
    // if (!ctx->calc_collum) return RUNTIME_ERROR;
    //
    // // get target collum_idx
    // size_t collum_idx = -1;
    // for (size_t i = 0; i < COLLUMS_COUNT; ++i)
    //     if (!strcmp(ctx->load_table[0][i], ctx->calc_collum)) {
    //         collum_idx = i;
    //         break;
    //     }
    // if (collum_idx == -1) return RUNTIME_ERROR;
    //
    // // handle special cases
    // if (ctx->load_table_len == 2)      // 1 elem = error, wiki: https://en.wikipedia.org/wiki/Median
    //     return NOT_ENOUGH_INFO;
    //
    // if (ctx->load_table_len == 3)
    // {
    //     double min = atof(ctx->load_table[0][collum_idx]);
    //     double max = atof(ctx->load_table[0][collum_idx]);
    //     if (min > max) {
    //         double tmp = min;
    //         min = max;
    //         max = tmp;
    //     }
    //     ctx->max = max;
    //     ctx->min = min;
    //     ctx->mid = (max + min) / 2.0;
    //     return SUCCESS;
    // } // 2 elems
    //
    // // handle other cases
    //
    // // qDebug("Count: %lu, Calculated capacity: %lu\n", ctx->load_table_len, ARR_INIT_SIZE * (size_t)pow(2, (int)ceil(log2(ctx->load_table_len / ARR_INIT_SIZE))));
    //
    // // fill sorted table
    // Array* sorted_table = get_array();
    // if (!sorted_table) return RUNTIME_ERROR;
    // // peek only calc region
    // if (!ctx->calc_region || !strcmp(ctx->calc_region, "All"))
    // {
    //     for (size_t i = 1; i < ctx->load_table_len; i++)
    //         push(sorted_table, ctx->load_table[i][collum_idx]);
    //
    // } else {
    //     for (size_t i = 1; i < ctx->load_table_len; i++)
    //     {
    //         if (!strcmp(ctx->load_table[i][REGION_COLLUM_NUM-1], ctx->calc_region))
    //         {
    //             push(sorted_table, ctx->load_table[i][collum_idx]);
    //         }
    //     }
    // }
    //
    // // sort table
    // for (size_t i = 0; i < sorted_table->count - 1; ++i)
    // {
    //     for (size_t j = i + 1; j < sorted_table->count; ++j)
    //     {
    //         if (atof((char*)sorted_table->data[i]) > atof((char*)sorted_table->data[j]))
    //         {
    //             void* tmp = sorted_table->data[i];
    //             sorted_table->data[i] = sorted_table->data[j];
    //             sorted_table->data[j] = tmp;
    //         }
    //     }
    // }
    //
    // // insert to context
    // double min = atof((char*)sorted_table->data[0]);
    // double max = atof((char*)sorted_table->data[sorted_table->count - 1]);
    //
    // double mid;
    // if (sorted_table->count % 2 == 0)
    // {
    //     double l = atof((char*)sorted_table->data[sorted_table->count/2]);
    //     double r = atof((char*)sorted_table->data[sorted_table->count/2-1]);
    //     mid = (l + r) / 2.0;
    // } else {
    //     mid = atof((char*)sorted_table->data[sorted_table->count/2]);
    // }
    //
    // ctx->max = max;
    // ctx->min = min;
    // ctx->mid = mid;
    //
    // // for (size_t i = 0; i < sorted_table->count; ++i){
    // //     qDebug("Region: |%s|\n", get_charpp(sorted_table,i)[1]);
    // // }
    //
    // return SUCCESS;
}

Result clear_context(AppContext* ctx, CLEAR_TARGET clear_target)
{
    if (!ctx) return RUNTIME_ERROR;

    switch (clear_target)
    {
        case OPEN_UI_DATA:
            if (ctx->filename) {
                free((char *) ctx->filename);
                ctx->filename = NULL;
            }

            if (ctx->table_header) {
                free(ctx->table_header);
                ctx->table_header = NULL;
                ctx->collums_count = 0;
            }

            if (ctx->table_content) {
                for (size_t i = 0; i < ctx->table_content_len; i++)
                    free(ctx->table_content[i]);
                free(ctx->table_content);
                ctx->table_content = NULL;
                ctx->table_content_len = 0;
            }

            if (ctx->table_all_regions) {
                free(ctx->table_all_regions);
                ctx->table_all_regions = NULL;
                ctx->regions_count = 0;
            }
            break;

        case LOAD_UI_DATA:
            if (ctx->region_to_load)
            {
                free((char *) ctx->region_to_load);
                ctx->region_to_load = NULL;
            }

            if (ctx->filtered_table)
            {
                // free(ctx->filtered_table);
                ctx->filtered_table = NULL;
                ctx->filtered_table_len = 0;
            }
            break;

        case CALC_UI_DATA:
            if (ctx->region_to_calc) {
                free(ctx->region_to_calc);
                ctx->region_to_calc = NULL;
            }

            if (ctx->collum_to_calc) {
                free(ctx->collum_to_calc);
                ctx->collum_to_calc = NULL;
            }
            break;

        case NOTHING:
            break;
    }

    return SUCCESS;
}

