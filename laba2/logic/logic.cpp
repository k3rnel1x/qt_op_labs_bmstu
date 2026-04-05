// TODO split this file

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

// incapsulated in logic.cpp
int set_pointers(char** ptrs, size_t len, char* str)
{
    size_t i = 0;
    // for (char* ptr = str; *ptr && i < COLLUMS_COUNT;)
    for (char* ptr = str; *ptr;)
    {
        char* print_ptr = ptr;
        // while (*print_ptr == ' ') print_ptr++;

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

Result parse_table(AppContext* ctx)
{
    // ######## checks ########
    if (!ctx) return RUNTIME_ERROR;

    FILE* f = fopen(ctx->filename, "r");
    if (!f) return NO_FILE;
    if (feof(f) || getc(f) == EOF) return EMPTY_FILE;
    rewind(f);

    // ######## init ########

    // init arrays
    Array* table   = get_array(); // char**
    if (!table) return RUNTIME_ERROR;
    Array* collums = get_array(); // char*
    if (!collums) return RUNTIME_ERROR;

    size_t parsed_raw_size = COLLUMS_COUNT * sizeof(char*) + STR_SIZE;

    // ######## start parsing ########
    clock_t start_time = clock();

    // parse header separately
    {
        char** raw = (char**)calloc(parsed_raw_size, sizeof(char));
        char*  str = (char*)(raw + COLLUMS_COUNT);

        // fill header
        fgets(str, STR_SIZE, f);

        if (!set_pointers(raw, COLLUMS_COUNT, str)) {
            free(raw);
            delete_arr(&table); delete_arr(&collums);
            return INVALID_HEADER;
        }

        for (size_t i = 0; i < COLLUMS_COUNT; i++)
            if (!raw[i]) {
                free(raw);
                delete_arr(&table); delete_arr(&collums);
                return INVALID_HEADER;
            }

        // insert header to table and collums
        push(table, raw);
        for (int i = 0; i < COLLUMS_COUNT; i++)
            push(collums, raw[i]);
    }

    Array* regions = get_array(); // char*
    if (!regions) return RUNTIME_ERROR;

    // parse all other
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
        if (!set_pointers(raw, COLLUMS_COUNT, str)) {
            free(raw);
            // errors_count++;
            continue; // skip line if invalid
        }

        // put to table
        push(table, raw);

        // put region if new
        int new_region = 1;
        for (size_t i = 0; new_region && i < regions->count; i++)
            new_region = strcmp(  get_charp(regions, i), raw[REGION_COLLUM_NUM-1] );

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

Result calc_metrix(AppContext* ctx)
{
    // qDebug() << "Calc metrix!";
    return SUCCESS;
}

Result clear_context(AppContext* ctx)
{
    if (!ctx) return RUNTIME_ERROR;

    if (ctx->filename) {
        free((char*)ctx->filename);
        ctx->filename = NULL;
    }

    // if (ctx->region_filter) {
    //     free((char*)ctx->region_filter);
    //     ctx->region_filter = NULL;
    // }

    if (ctx->table) {
        for (int i = 0; i < ctx->table_len; i++)
            free(ctx->table[i]);
        free(ctx->table);
        ctx->table = NULL;
        ctx->table_len = 0;
    }

    if (ctx->regions) {
        for (int i = 0; i < ctx->regions_count; i++)
            free(ctx->regions[i]);
        free(ctx->regions);
        ctx->regions = NULL;
        ctx->regions_count = 0;
    }

    if (ctx->collums) {
        for (int i = 0; i < ctx->collums_count; i++)
            free(ctx->collums[i]);
        free(ctx->collums);
        ctx->collums = NULL;
        ctx->collums_count = 0;
    }

    if (ctx->calc_collum) {
        free(ctx->calc_collum);
        ctx->calc_collum = NULL;
    }

    if (ctx->calc_region) {
        free(ctx->calc_region);
        ctx->calc_region = NULL;
    }

    return SUCCESS;
}

// TODO implement calc_metrix with all cases!!