//
// Created by k3rnel1x on 05.04.2026.
//
#include "entrypoint.h"
#include "logic.h"

Result perform_operation(OperationType type, AppContext* ctx, Params* params)
{
    if(!ctx) return RUNTIME_ERROR;
    Result res_code = SUCCESS;

    switch (type) {
        case OPEN_TABLE:
            res_code = open_table(ctx);
            break;
        case LOAD_TABLE:
            res_code = load_table(ctx);
            break;
        case CALC_METRIX:
            res_code = calc_metrix(ctx);
            break;
        case CLEAR_CONTEXT:
            if (!params) return RUNTIME_ERROR;
            res_code = clear_context(ctx, params->clear_target);
            break;
    }
    return res_code;
}