//
// Created by k3rnel1x on 30.03.2026.
//

#ifndef LOGIC_H
#define LOGIC_H

#include "appcontext.h"
#include "result.h"

typedef enum CLEAR_TARGET {
    NOTHING,
    OPEN_UI_DATA,
    LOAD_UI_DATA,
    CALC_UI_DATA
} CLEAR_TARGET;

Result open_table(AppContext* ctx);
Result load_table(AppContext* ctx);
Result calc_metrix(AppContext* ctx);
Result clear_context(AppContext* ctx, CLEAR_TARGET clear_target);

#endif //LOGIC_H
