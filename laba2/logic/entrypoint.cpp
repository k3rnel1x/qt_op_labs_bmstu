//
// Created by k3rnel1x on 05.04.2026.
//
#include "entrypoint.h"
#include "logic.h"

Result perform_operation(OperationType type, AppContext* ctx, Params* ui_params)
{
    if(!ctx || /*(type != CLEAR_CONTEXT && !ui_params) ||*/ (type == ERASE_CONTEXT && !ui_params)) return RUNTIME_ERROR;
    Result res_code = SUCCESS;

    switch (type) {
        case OPEN_TABLE:
            res_code = open_table(ctx, ui_params);
            break;
        case LOAD_TABLE:
            res_code = load_table(ctx, ui_params);
            break;
        case CALC_METRIX:
            res_code = calc_metrix(ctx);
            break;
        case ERASE_CONTEXT:
            res_code = erase_context(ctx);
            break;
    }
    return res_code;
}
