#ifndef LOGIC_H
#define LOGIC_H

#define NAME_COLLUM_NUM 3
#define BUFF_SIZE       1024
#define MAX_COLUM_IDX   7
#define HEADER          "year,region,natural_population_growth,birth_rate,death_rate,general_demographic_weight,urbanization\n"

#include "result.h"
#include "appcontext.h"

result_code load_table(AppContext* ctx);
result_code calc_metrix(AppContext* ctx);

#endif // LOGIC_H
