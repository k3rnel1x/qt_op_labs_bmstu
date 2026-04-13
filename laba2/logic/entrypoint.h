#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "result.h"
#include "appcontext.h"

typedef enum OperationType {
    OPEN_TABLE,
    LOAD_TABLE,
    CALC_METRIX,
    ERASE_CONTEXT
} OperationType;

typedef enum CLEAR_TARGET {
    NOTHING,
    OPEN_UI_DATA,
    LOAD_UI_DATA,
    CALC_UI_DATA
} CLEAR_TARGET;


typedef struct Params {

    // OPEN_TABLE data
    const char* filename;

    // LOAD_TABLE data
    const char* region_to_load;

    // CALC_METRIX data
    const char* region_to_calc;
    const char* collum_to_calc;

    // CLEAR_CONTEXT data
    // CLEAR_TARGET clear_target;

} Params;

Result perform_operation(OperationType type, AppContext* ctx, Params* ui_params);

#endif // ENTRYPOINT_H
