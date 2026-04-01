#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "logic/result.h"
#include "logic/logic.h"

typedef enum OperationType {
    PARCE_TABLE,
    CALC_METRIX
} OperationType;

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
    }
    return res_code;
}

#endif // ENTRYPOINT_H
