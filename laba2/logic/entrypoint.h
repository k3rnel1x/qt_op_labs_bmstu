#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "result.h"
#include "appcontext.h"
#include "logic.h"

typedef enum OperationType {
    PARCE_TABLE,
    CALC_METRIX,
    GET_LOAD_TABLE,
} OperationType;

Result perform_operation(OperationType type, AppContext* ctx);

#endif // ENTRYPOINT_H
