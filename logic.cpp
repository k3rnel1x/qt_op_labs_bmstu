#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QDebug>

#define MEM_ERROR errors_count++
// #define MEM_ERROR NULL;

#include "appcontext.h"
#include "utils.h"
#include "vector.h"
#include "result.h"
#include "logic.h"

#define BUFF_SIZE 1024

result_code load_table(AppContext* ctx)
{
    // if(!VALID_APPCONTEXT(ctx)) return ERROR;
    if(!ctx) return ERROR;
    if(!ctx->choised_region) return ERROR;
    if(!ctx->opened_file) return ERROR;

    FILE* f = ctx->opened_file;

    size_t curr_line = 0;

    char* buff = (char*)calloc(BUFF_SIZE, sizeof(char));
    if (!buff) return ERROR;

    if(!fgets(buff, BUFF_SIZE-1, f)) return FILE_EMPTY;
    curr_line++;

    Vector v = v_init();
    size_t  errors_count = 0;
    size_t  max_name_size = 0;
    size_t  total_count = 0;

    while (!feof(f))
    {
        memset(buff, 0, BUFF_SIZE);
        if (!fgets(buff, BUFF_SIZE-1, f))
            continue;
        curr_line++;

        if ((!feof(f) && buff[strlen(buff)-1] != '\n') || char_count(buff, ',') != 6){
            MEM_ERROR;
            continue;
        }
        if (buff[0] == ','){
            MEM_ERROR;
            continue;
        }

        if (strstr(buff, ",,") || strstr(buff, ",\n")){
            MEM_ERROR;
            continue;
        }

        *strchr(buff, '\n') = 0;

        Line* line = get_line();
        if (!line) return ERROR;
        char* ptr = buff;

        // start filling

        // fill year
        {
            if (!fill_double(&line->year, ptr))
            {
                MEM_ERROR;
                free(line);
                continue;
            }
            // move
            ptr = strchr(ptr, ',') + 1;
        }



        // fill region
        {
            line->region = fill_str(ptr);
            if (line->region == NULL) {
                MEM_ERROR;
                free(line);
                continue;
            }
            max_name_size = max_name_size < strlen(line->region)? strlen(line->region) : max_name_size;
            // move
            ptr = strchr(ptr, ',') + 1;
        }



        // fill other fields
        double* fields[] = {
            &line->natural_grow,
            &line->birth_rate,
            &line->death_rate,
            &line->general_demographic_weight,
            &line->urbanization
        };

        for (int i = 0; i < sizeof(fields)/sizeof(double*); i++)
        {
            if (!fill_double(fields[i], ptr))
            {
                MEM_ERROR;
                free(line->region);
                free(line);
                line = NULL;
                break;
            }
            // move
            ptr = strchr(ptr, ',') + 1;
        }

        if (line)
            v_push_ptr(&v, line);
    }
    free(buff);

    if(curr_line == 1) return TABLE_EMPTY;

    // push to appcontext
    ctx->errors_count = errors_count;
    ctx->lines = v;
    return SUCCESS;
}

result_code calc_metrix(AppContext* ctx)
{
    if(!VALID_APPCONTEXT(ctx))   return ERROR;
    if(ctx->choised_collum == 2) return NAME_COLLUM;

    Vector* v = &ctx->lines;
    size_t user_choice = ctx->choised_collum - 1;

    double* min = v_item(v, 0)->by_idx[user_choice];
    double* max = v_item(v, 0)->by_idx[user_choice];
    for(int i = 0; i < v->count; i++)
    {
        Line* line = v_item(v, i);
        if(strcmp(line->region, ctx->choised_region) == 0)
        {
            double* line_choised_collum = line->by_idx[user_choice];
            *min = *min < *line_choised_collum? *min : *line_choised_collum;
            *max = *max > *line_choised_collum? *max : *line_choised_collum;
        }
    }

    ctx->min = *min;
    ctx->max = *max;
    ctx->mid = (*max + *min) / 2.0;
}
