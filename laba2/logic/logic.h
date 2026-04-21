//
// Created by k3rnel1x on 30.03.2026.
//

#ifndef LOGIC_H
#define LOGIC_H

#include "appcontext.h"
#include "entrypoint.h"
#include "result.h"

// #### Parse config ####
#define STR_SIZE 200
#define SEP ','

Result open_table(AppContext*  ctx, Params* p);
Result load_table(AppContext*  ctx, Params* p);
Result calc_metrix(AppContext* ctx, Params* p);
Result erase_context(AppContext* ctx);
Result get_year_sorted_table(AppContext* ctx);

#endif //LOGIC_H
