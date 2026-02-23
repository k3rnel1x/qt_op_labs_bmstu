#include "entrypoint.h"
#include "logic.h"

void updateClickCount(Operation operation, AppContext* context);

void doOperation(Operation operation, AppContext* ctx)
{
    switch(operation) {
    case CONVERT:
        doConvert(ctx);
        break;

    case INIT:
        doInit(ctx);
        break;

    case DEINIT:
        doDeInit(ctx);
        break;

    case CLEAR:
        doClear(ctx);
        break;
    }
}
