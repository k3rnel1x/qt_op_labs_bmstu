#include "entrypoint.h"
#include "logic.h"

void updateClickCount(Operation operation, AppContext* context);

void doOperation(Operation operation, AppContext* ctx) {
    switch(operation) {
    case Convert:
        convert(ctx);
        break;
    case Initialization:
        initialize(ctx);
        break;
    case DeInitialization:
        deinitialize(ctx);
        break;
	}
}
