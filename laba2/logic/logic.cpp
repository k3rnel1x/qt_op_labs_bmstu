// TODO split this file

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "appcontext.h"
#include "result.h"

#define STR_SIZE 200
#define SEP ','

// #define TABLE_BASE_LENGHT   100
// #define TABLE_LENGHT_SCALE 2

// #define REGIONS_BASE_LENGHT 100
// #define REGIONS_LENGHT_SCALE 2
#define REGION_COLLUM_NUM 2

#define ARR_INIT_SIZE  100
#define ARR_SIZE_SCALE 2

struct Array {
    size_t capacity;
    size_t count;
    void** data;
};

Array* get_array()
{
    Array* arr = (Array*)calloc(1, sizeof(Array));
    if (!arr) return NULL;
    arr->data = (void**)calloc(ARR_INIT_SIZE, sizeof(void*));
    if (!arr->data) return NULL;

    arr->capacity = ARR_INIT_SIZE;
    arr->count = 0;
    return arr;
}

void push(Array* arr, void* item)
{
    if (!arr || !item) return;

    // resize if needed
    if (arr->capacity == arr->count) {
        void** old_data = arr->data;
        arr->data = (void**)realloc(arr->data, (arr->capacity *= ARR_SIZE_SCALE) * sizeof(void*));
        if (!arr->data) {
            arr->data = old_data;
            return;
        }
    }

    // insert item
    arr->data[arr->count++] = item;
}

void delete_arr(Array** arr_ptr)
{
    if (!arr_ptr) return;

    Array* arr = *arr_ptr;

    for (int i = 0; i < arr->count; i++) {
        free(arr->data[i]);
    }
    free(arr->data);
    free(arr);

    arr = NULL;
}

inline
char** get_charpp(Array* arr, size_t index)
{
    if (index >= arr->count) return NULL;
    return (char**)arr->data[index];
}

inline
char* get_charp(Array* arr, size_t index)
{
    if (index >= arr->count) return NULL;
    return (char*)arr->data[index];
}

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

Result parse_table(AppContext* ctx)
{
    // ### checks ###
    if (!ctx) return RUNTIME_ERROR;

    // ### clear ###
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

    // ### init ###
    // init arrays
    Array* table   = get_array(); // char**
    if (!table) return RUNTIME_ERROR;
    Array* regions = get_array(); // char*
    if (!regions) return RUNTIME_ERROR;
    Array* collums = get_array(); // char*
    if (!collums) return RUNTIME_ERROR;

    // ### start parsing ###
    clock_t start = clock();
    const char* region_filter = NULL;
    if (ctx->region_filter && *ctx->region_filter)
        region_filter = ctx->region_filter;

    size_t parsed_str_size = COLLUMS_COUNT * sizeof(char*) + STR_SIZE;

    // # parse header #
    {
        char** raw = (char**)calloc(parsed_str_size, sizeof(char));
        char*  str = (char*)(raw + COLLUMS_COUNT);

        // fill header
        fgets(str, STR_SIZE, f);

        if (!set_pointers(raw, COLLUMS_COUNT, str)) {
            free(raw);
            delete_arr(&table); delete_arr(&regions); delete_arr(&collums);
            return INVALID_HEADER;
        }

        for (size_t i = 0; i < COLLUMS_COUNT; i++)
            if (!raw[i]) {
                free(raw);
                delete_arr(&table); delete_arr(&regions); delete_arr(&collums);
                return INVALID_HEADER;
            }

        // insert header to table and collums
        push(table, raw);
        for (int i = 0; i < COLLUMS_COUNT; i++)
            push(collums, raw[i]);
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
            // errors_count++;
            continue; // skip line if invalid
        }

        // filtering
        if (region_filter && strcmp(raw[REGION_COLLUM_NUM-1], region_filter) != 0 )
            continue;

        // put to table
        push(table, raw);
        // ctx->progress_value++;

        // put region if new
        int new_region = 1;
        for (size_t i = 0; new_region && i < regions->count; i++)
            new_region = strcmp(  get_charp(regions, i), raw[REGION_COLLUM_NUM-1] );

        if (new_region)
            push(regions, raw[REGION_COLLUM_NUM-1]);
    } while (!feof(f));

    // stop timer //
    clock_t end = clock();
    double parse_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    if (regions->count == 0)
        delete_arr(&regions);

    // ### push results to context ###
    ctx->table = (char***)table->data;
    ctx->table_len = table->count;
    ctx->parse_time = parse_time;

    ctx->regions = (char**)regions->data? (char**)regions->data : NULL;
    ctx->regions_count = regions? regions->count : 0;

    ctx->collums = (char**)collums->data;
    ctx->collums_count = COLLUMS_COUNT;
    return SUCCESS;
}

Result calc_metrix(AppContext* ctx)
{
    qDebug() << "Calc metrix!";
    return SUCCESS;
}

// TODO implement calc_metrix with all cases!!