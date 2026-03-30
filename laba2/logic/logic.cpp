// TODO split this file


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "appcontext.h"
#include "result.h"

#define STR_SIZE 200
#define TABLE_BASE_LENGHT 100
#define TABLE_LENGHT_SCALE 2

int set_pointers(char** ptrs, size_t len, char* str)
{
    static char sep = ',';
    size_t i = 0;
    // for (char* ptr = str; *ptr && i < COLLUMS_COUNT;)
    for (char* ptr = str; *ptr;)
    {
        char* print_ptr = ptr;
        // while (*print_ptr == ' ') print_ptr++;

        while (*ptr && *ptr != sep && *ptr != '\n') ptr++;
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

Result load_table(AppContext* ctx)
{
    clock_t start = clock();

    // ### checks ###
    // regions and table must be non-allocated
    if (!ctx || ctx->regions || ctx->table) return RUNTIME_ERROR;

    FILE* f = fopen(ctx->filename, "r");
    if (!f) return NO_FILE;
    if (feof(f)) return EMPTY_FILE;

    // start timer //

    // ### create table ###
    size_t table_capacity = TABLE_BASE_LENGHT;
    char*** table = (char***)malloc(table_capacity*sizeof(char**));
    if (!table) return RUNTIME_ERROR;
    size_t errors_count = 0;



    // ### start parsing ###
    size_t parsed_str_size = COLLUMS_COUNT * sizeof(char*) + STR_SIZE;
    size_t table_idx = 0;

    // ## parse header ##
    {
        char** raw = (char**)calloc(parsed_str_size, sizeof(char));
        char*  str = (char*)(raw + COLLUMS_COUNT);

        // fill header
        fgets(str, STR_SIZE, f);

        if (!set_pointers(raw, COLLUMS_COUNT, str)) {
            free(raw);
            free(table);
            return INVALID_HEADER;
        }

        for (size_t i = 0; i < COLLUMS_COUNT; i++)
            if (!raw[i]) {
                free(raw);
                free(table);
                return INVALID_HEADER;
            }
        table[table_idx++] = raw;
    }

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

        // resize table if needed
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

        // put to table
        table[table_idx++] = raw;
    } while (!feof(f));

    // stop timer //
    clock_t end = clock();
    double time = ((double) (end - start)) / CLOCKS_PER_SEC;

    // ### push results to context ###
    ctx->table = table;
    ctx->table_len = table_idx;
    ctx->load_time = time;
    return SUCCESS;
}


// TODO implement calc_metri with all cases!!