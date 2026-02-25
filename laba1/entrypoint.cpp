#include "entrypoint.h"
#include "logic.h"

Result doOperation(Operation operation, AppContext* ctx)
{
    Result result = SUCCEED;
    switch(operation) {
    case CONVERT:
        result = doConvert(ctx);
        break;

    case INIT:
        result = doInit(ctx);
        break;

    case DEINIT:
        result = doDeInit(ctx);
        break;

    case CLEAR:
        result = doClear(ctx);
        break;

    case SWAP:
        result = doSwap(ctx);
        break;
    }

    return result;
}
