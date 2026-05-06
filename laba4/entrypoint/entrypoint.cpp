#include "entrypoint.h"
#include <businesslogic.h>

ResultCode performOperation(AppContext* context, Params* params, Operation type)
{
    if(!context) return ERROR;

    ResultCode result = SUCCEED;
    switch(type)
    {
    case LoadFile:
        result = loadFuncTable(context, params);
        break;

    case CleanUp:
        result = deleteContext(context);
        break;

    case UpdateStepConfig:
        result = updateStepConfig(context, params);
        break;

    case UpdateRangeConfig:
        result = updateRangeConfig(context, params);
        break;

    case NormalizePoints:
        result = calcNormalizedCoords(context);
        break;
    }

    return result;
}