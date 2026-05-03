#include "entrypoint.h"
#include <businesslogic.h>

ResultCode performOperation(AppContext* context, Params* params, Operation type)
{
    if(!context || (type != CleanUp && !params)) return ERROR;

    ResultCode result = SUCCEED;
    switch(type)
    {
    case LoadFile:
        result = loadFuncTable(context, params);
        break;

    case CleanUp:
        result = deleteContext(context);
        break;
    }

    return result;
}