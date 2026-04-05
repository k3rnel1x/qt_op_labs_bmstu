//
// Created by k3rnel1x on 05.04.2026.
//
#include "entrypoint.h"
#include "logic.h"

Result perform_operation(OperationType type, AppContext* ctx)
{
    if(!ctx) return RUNTIME_ERROR;
    Result res_code = SUCCESS;

    switch (type) {
        case PARCE_TABLE:
            res_code = parse_table(ctx);
            break;
        case CALC_METRIX:
            res_code = calc_metrix(ctx);
            break;
        case CLEAR_CTX:
            res_code = clear_context(ctx);
            break;
        case GET_LOAD_TABLE:
            res_code = get_load_table(ctx);
            break;
    }
    return res_code;
}