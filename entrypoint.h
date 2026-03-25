#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "logic.h"

typedef enum OperationType {
    LOAD_TABLE,
    CALC_METRIX
} OperationType;

result_code perform_operation(OperationType type, AppContext* ctx)
{
    if(!ctx) return ERROR;
    result_code res_code = SUCCESS;

    switch (type) {
    case LOAD_TABLE:
        res_code = load_table(ctx);
        break;
    case CALC_METRIX:
        res_code = calc_metrix(ctx);
        break;
    }
    return res_code;
}

#endif // ENTRYPOINT_H
