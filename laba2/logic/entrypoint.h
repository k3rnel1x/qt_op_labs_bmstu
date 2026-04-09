#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "result.h"
#include "appcontext.h"
#include "logic.h"

typedef enum OperationType {
    OPEN_TABLE,
    LOAD_TABLE,
    CALC_METRIX,
    CLEAR_CONTEXT
} OperationType;


typedef struct Params {
    CLEAR_TARGET clear_target;
} Params;

Result perform_operation(OperationType type, AppContext* ctx, Params* params);

#endif // ENTRYPOINT_H
