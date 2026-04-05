//
// Created by k3rnel1x on 30.03.2026.
//

#ifndef LOGIC_H
#define LOGIC_H

#include "appcontext.h"
#include "result.h"
#include "entrypoint.h"

Result parse_table(AppContext* ctx);
Result calc_metrix(AppContext* ctx);
Result clear_context(AppContext* ctx);
Result get_load_table(AppContext* ctx);

#endif //LOGIC_H
