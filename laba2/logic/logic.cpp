// TODO split this file


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "appcontext.h"
#include "result.h"

#define STR_SIZE 200
#define SEP ','

#define TABLE_BASE_LENGHT   100
#define TABLE_LENGHT_SCALE 2

#define REGIONS_BASE_LENGHT 100
#define REGIONS_LENGHT_SCALE 2
#define REGION_COLLUM_NUM 2

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

Result parse_table(AppContext* ctx)
{

    // ### checks ###
    if (!ctx) return RUNTIME_ERROR;

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

    FILE* f = fopen(ctx->filename, "r");
    if (!f) return NO_FILE;
    if (feof(f)) return EMPTY_FILE;


    // ### init table, regions, collums ###
    size_t table_capacity = TABLE_BASE_LENGHT;
    char*** table = (char***)malloc(table_capacity*sizeof(char**));
    if (!table) return RUNTIME_ERROR;
    size_t errors_count = 0;

    size_t regions_capacity = REGIONS_BASE_LENGHT;
    char** regions = (char**)calloc(regions_capacity, sizeof(char*));
    if (!regions) return RUNTIME_ERROR;

    char** collums = (char**)calloc(COLLUMS_COUNT, sizeof(char*));
    if (!collums) return RUNTIME_ERROR;

    // ### start parsing ###
    clock_t start = clock();
    const char* region_filter = NULL;
    if (ctx->region_to_load && *ctx->region_to_load)
        region_filter = ctx->region_to_load;

    size_t parsed_str_size = COLLUMS_COUNT * sizeof(char*) + STR_SIZE;
    size_t table_idx = 0;
    size_t region_idx = 0;

    // # parse header #
    {
        char** raw = (char**)calloc(parsed_str_size, sizeof(char));
        char*  str = (char*)(raw + COLLUMS_COUNT);

        // fill header
        fgets(str, STR_SIZE, f);

        if (!set_pointers(raw, COLLUMS_COUNT, str)) {
            free(raw);
            free(table);
            free(regions);
            free(collums);
            return INVALID_HEADER;
        }

        for (size_t i = 0; i < COLLUMS_COUNT; i++)
            if (!raw[i]) {
                free(raw);
                free(table);
                free(regions);
                free(collums);
                return INVALID_HEADER;
            }

        // insert header to table and collums
        table[table_idx++] = raw;
        for (int i = 0; i < COLLUMS_COUNT; i++)
            collums[i] = raw[i];
    }

    // # parse lines #
    // ctx->progress_value = 0;
    do{
        // alloc new line
        char** raw = (char**)calloc(parsed_str_size, sizeof(char));
        char*  str = (char*)(raw + COLLUMS_COUNT);

        // fill str
        if (!fgets(str, STR_SIZE, f)) {
            free(raw);
            break;
        }

        // parse str
        if (!set_pointers(raw, COLLUMS_COUNT, str)) {
            free(raw);
            errors_count++;
            continue; // skip line if invalid
        }

        // filtering
        if (region_filter && strcmp(raw[REGION_COLLUM_NUM-1], region_filter) != 0 )
            continue;

        // resize table and regions if needed
        if (table_idx == table_capacity) {
            char*** old_table = table;
            table = (char***)realloc(old_table, (table_capacity *= TABLE_LENGHT_SCALE) * sizeof(char**));
            if (!table) {
                for (size_t i = 0; i < table_idx; i++)
                    free(old_table[i]);
                free(old_table);
                free(raw);
                return RUNTIME_ERROR;
            }
        }

        if (region_idx == regions_capacity) {
            char** old_regions = regions;
            regions = (char**)realloc(old_regions, (regions_capacity *= REGIONS_LENGHT_SCALE) * sizeof(char*));
            if (!regions) {
                for (size_t i = 0; i < region_idx; i++)
                    free(old_regions[i]);
                free(old_regions);
                return RUNTIME_ERROR;
            }
        }

        // put to table
        table[table_idx++] = raw;
        // ctx->progress_value++;

        // put region if new
        int new_region = 1;
        for (size_t i = 0; new_region && i < region_idx; i++)
            new_region = strcmp( regions[i], raw[REGION_COLLUM_NUM-1] );

        if (new_region)
            regions[region_idx++] = raw[REGION_COLLUM_NUM-1];
    } while (!feof(f));

    // stop timer //
    clock_t end = clock();
    double parse_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    if (region_idx == 0){
        free(regions);
        regions = NULL;
    }

    // ### push results to context ###
    ctx->table = table;
    ctx->table_len = table_idx;
    ctx->parse_time = parse_time;

    ctx->regions = regions;
    ctx->regions_count = region_idx;

    ctx->collums = collums;
    ctx->collums_count = COLLUMS_COUNT;
    return SUCCESS;
}

Result calc_metrix(AppContext* ctx)
{
    qDebug() << "Calc metrix!";
    return SUCCESS;
}

// TODO implement calc_metrix with all cases!!