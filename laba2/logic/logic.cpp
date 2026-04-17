#include "logic.h"

#include <cinttypes>
#include <qlogging.h>
#include <QDebug>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "appcontext.h"
#include "entrypoint.h"
#include "result.h"
#include "array.h"

#include "../config.h"

typedef double metrix_t;

void swapp(void** ptr1, void** ptr2)
{
    if(!ptr1 || !ptr2) return;

    void* tmp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = tmp;
}

void swapp(double** ptr1, double** ptr2)
{
    if(!ptr1 || !ptr2) return;

    double* tmp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = tmp;
}

int partition(double** a, int start, int end)
{
    double* pivot = a[end];
    int pIndex = start;
    for (int i = start; i < end; ++i)
    {
        if (a[i] > pivot)
        {
            swapp(a + i, a + pIndex);
            ++pIndex;
        }
    }

    swapp (a + pIndex, a + end);
    return pIndex;
}

int partition(AppContext* ctx, void** a, int start, int end, int (*cmp)(void*, void*, AppContext*))
{
    void* pivot = a[end];
    int pIndex = start;
    for (int i = start; i < end; ++i)
    {
        if (cmp(a[i], pivot, ctx) > 0)
        {
            swapp(a + i, a + pIndex);
            ++pIndex;
        }
    }

    swapp (a + pIndex, a + end);
    return pIndex;
}

void quicksort(double** a, int start, int end)
{
    if (start >= end)  return;
    int pivot = partition(a, start, end);
    quicksort(a, start, pivot - 1);
    quicksort(a, pivot + 1, end);
}

void quicksort(AppContext* ctx, void** a, int start, int end, int (*cmp)(void*, void*, AppContext*))
{
    if (start >= end)  return;
    int pivot = partition(ctx, a, start, end, cmp);
    quicksort(ctx, a, start, pivot - 1, cmp);
    quicksort(ctx, a, pivot + 1, end, cmp);
}

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
    case CALC_UI_DATA:
        ctx->calculated_collum_idx = -1;
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

    case CALC_VISUALIZATION_DATA:
        if (ctx->year_sorted_table)
        {
            free(ctx->year_sorted_table);
            ctx->year_sorted_table = NULL;
            ctx->year_sorted_table_len = 0;
        }
        break;

    case NOTHING:
        break;
    }

    return SUCCESS;
}

// Operations
Result open_table(AppContext* ctx, Params* p)
{
    // ######## checks ########
    if (!ctx || !p || !p->filename) return RUNTIME_ERROR;

    FILE *f = fopen(p->filename, "r");
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
        parsed_raw_size = collums_count * sizeof(char*) + STR_SIZE + sizeof(metrix_t);

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

    if (table_content->count == 0)
        delete_arr(&table_content);


    // ### push results to context ###
    clear_context(ctx, OPEN_UI_DATA);
    if (ctx->filename)
        free((char*)ctx->filename);
    ctx->filename = p->filename;
    p->filename = NULL;

    ctx->table_header  = table_header;
    ctx->collums_count = collums_count;

    ctx->table_content = table_content? (char***)table_content->data : NULL;
    ctx->table_content_len = table_content? table_content->count : 0;

    ctx->table_all_regions = table_regions? (char**)table_regions->data : NULL;
    ctx->regions_count = table_regions? table_regions->count : 0;

    ctx->parse_time = parse_time;
    ctx->errors_count = errors_count;

    ctx->total_raw_size = parsed_raw_size;
    return SUCCESS;
}

Result load_table(AppContext* ctx, Params* p)
{
    if (!ctx || !p || !p->region_to_load) return RUNTIME_ERROR;

    // All regions
    /// ####################################################
    if (!strcmp(p->region_to_load, "All"))
    {
        ctx->filtered_table = ctx->table_content;
        ctx->filtered_table_len = ctx->table_content_len;
        ctx->region_to_load = p->region_to_load;
        p->region_to_calc = NULL;
        return SUCCESS;
    }

    // Filtering....
    /// ####################################################
    const char* region_to_load = p->region_to_load;
    char*** filtered_table = NULL;
    size_t  filtered_table_len = 0;
    Array* load_table = get_array();
    if (!load_table) return RUNTIME_ERROR;

    for (int i = 0; i < ctx->table_content_len; ++i) {
        if (strcmp(ctx->table_content[i][REGION_COLLUM_NUM-1], region_to_load) == 0) {
            push(load_table, ctx->table_content[i]);
        }
    }
    /// ####################################################

    // Nothing match
    if (load_table->count == 0) {
        delete_arr(&load_table);
        filtered_table = NULL;
        filtered_table_len = 0;
    // other match
    } else {
        filtered_table = (char***)load_table->data;
        filtered_table_len = load_table->count;
    }

    // insert to context
    /// ####################################################
    clear_context(ctx, LOAD_UI_DATA);
    ctx->region_to_load = region_to_load;
    p->region_to_calc = NULL;
    ctx->filtered_table = filtered_table;
    ctx->filtered_table_len = filtered_table_len;
    return SUCCESS;
}

int double_cmp(void* a, void* b) {return *(double*)a > *(double*)b; }

Result calc_metrix(AppContext* ctx, Params* p)
{
    if (!ctx || !ctx->filtered_table || !ctx->filtered_table_len) return RUNTIME_ERROR;
    if (!p || !p->region_to_calc || !p->collum_to_calc) return RUNTIME_ERROR;


    // ########################### get target collum_idx ############################
    size_t collum_idx = -1;
    for (size_t i = 0; i < ctx->collums_count; ++i)
        if (!strcmp(ctx->table_header[i], p->collum_to_calc)) {
            collum_idx = i;
            break;
        }
    if (collum_idx == -1) return RUNTIME_ERROR;

    // ########################## handle special cases ###############################
    // 1 elem = error, wiki: https://en.wikipedia.org/wiki/Median
    if (ctx->filtered_table_len == 1)
        return NOT_ENOUGH_INFO;

    // 2 elems
    if (ctx->filtered_table_len == 2 &&
        !strcmp(ctx->filtered_table[0][collum_idx], p->region_to_calc) &&
        !strcmp(ctx->filtered_table[1][collum_idx], p->region_to_calc))
    {
        double min = atof(ctx->filtered_table[0][collum_idx]);
        double max = atof(ctx->filtered_table[1][collum_idx]);
        if (min > max) {
            double tmp = min;
            min = max;
            max = tmp;
        }
        ctx->max = max;
        ctx->min = min;
        ctx->mid = (max + min) / 2.0;
        return SUCCESS;
    }

    // ############################## calc metrix #####################################

    // fill data field
    size_t byte_metrix_offset = ctx->collums_count * sizeof(char*) + STR_SIZE;
    for (size_t i = 0; i < ctx->filtered_table_len; ++i){
        metrix_t* ptr = (metrix_t*)((char*)ctx->filtered_table[i] + byte_metrix_offset);
        *ptr = atof(ctx->filtered_table[i][collum_idx]);
    }

    // construct sorted table
    Array* sorted_table = get_array();
    if (!sorted_table) return RUNTIME_ERROR;

    if (!strcmp(p->region_to_calc, "All"))
    {
        for (size_t i = 0; i < ctx->filtered_table_len; ++i)
            push(sorted_table, (double*)((char*)ctx->filtered_table[i] + byte_metrix_offset));

    } else {
        for (size_t i = 0; i < ctx->filtered_table_len; ++i)
            if (!strcmp(ctx->filtered_table[i][REGION_COLLUM_NUM-1], p->region_to_calc))
                push(sorted_table, (double*)((char*)ctx->filtered_table[i] + byte_metrix_offset));
    }

    if (sorted_table->count < 2)
    {
        free(sorted_table->data);
        free(sorted_table);
        return NOT_ENOUGH_INFO;
    }
    // use quicksort to sort table
    // quicksort((double**)sorted_table->data, 0, sorted_table->count - 1);

    // ############################ get values #################################
    // getting info
    double min = *(double*)sorted_table->data[0];
    double max = *(double*)sorted_table->data[sorted_table->count - 1];

    double mid;
    if (sorted_table->count % 2 == 0)
    {
        double l = *(double*)sorted_table->data[sorted_table->count/2];
        double r = *(double*)sorted_table->data[sorted_table->count/2-1];
        mid = (l + r) / 2.0;
    } else
        mid = *(double*)sorted_table->data[sorted_table->count/2];

    // clear
    free(sorted_table->data);
    free(sorted_table);

    // ######################### insert to context ###############################
    ctx->max = max;
    ctx->min = min;
    ctx->mid = mid;
    ctx->calculated_region = p->region_to_calc;
    ctx->calculated_collum = p->collum_to_calc;
    ctx->calculated_collum_idx = collum_idx;
    return SUCCESS;
}

int strcmp(void* raw1, void* raw2, AppContext* ctx)
{
    return *(double*)((char*)raw1 + ctx->total_raw_size - sizeof(metrix_t)) > *(double*)((char*)raw2 + ctx->total_raw_size - sizeof(metrix_t));
}

Result get_year_sorted_table(AppContext* ctx)
{
    if (!ctx || !ctx->filtered_table || !ctx->filtered_table_len) return RUNTIME_ERROR;
    if (!ctx->calculated_collum || !ctx->calculated_region) return RUNTIME_ERROR;

    clear_context(ctx, CALC_VISUALIZATION_DATA);
    Array* year_sorted_table = get_array();
    if (!year_sorted_table) return RUNTIME_ERROR;

    size_t byte_metrix_offset = ctx->collums_count * sizeof(char*) + STR_SIZE;
    for (size_t i = 0; i < ctx->filtered_table_len; ++i){
        metrix_t* ptr = (metrix_t*)((char*)ctx->filtered_table[i] + byte_metrix_offset);
        *ptr = atof(ctx->filtered_table[i][0]);
    }

    if (!strcmp(ctx->calculated_region, "All"))
    {
        void** old_data = year_sorted_table->data;
        year_sorted_table->data = (void**)realloc(year_sorted_table->data, sizeof(void*) * ctx->filtered_table_len);
        if(!year_sorted_table->data)
        {
            free(old_data);
            return RUNTIME_ERROR;
        }
        memcpy(year_sorted_table->data, ctx->filtered_table, sizeof(void*) * ctx->filtered_table_len);
        year_sorted_table->count = ctx->filtered_table_len;

    } else {
        for (size_t i = 0; i < ctx->filtered_table_len; ++i)
            if (!strcmp(ctx->filtered_table[i][REGION_COLLUM_NUM-1], ctx->calculated_region))
                push(year_sorted_table, ctx->filtered_table[i]);

    }

    quicksort(ctx, year_sorted_table->data, 0, year_sorted_table->count - 1, strcmp);


    // insert to context
    ctx->year_sorted_table = (char***)year_sorted_table->data;
    ctx->year_sorted_table_len = year_sorted_table->count;
    // qDebug() << ((char**)(year_sorted_table->data[3]))[2];

    free(year_sorted_table);
    return SUCCESS;
}

Result erase_context(AppContext* ctx)
{
    if(!ctx) return RUNTIME_ERROR;
    clear_context(ctx, OPEN_UI_DATA);
    clear_context(ctx, LOAD_UI_DATA);
    clear_context(ctx, CALC_UI_DATA);
    clear_context(ctx, CALC_VISUALIZATION_DATA);

    return SUCCESS;
}

