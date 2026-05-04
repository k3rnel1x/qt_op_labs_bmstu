//
// Created by k3rnel1x on 02.05.2026.
//

#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "../app/appcontext.h"

typedef enum Operation {
    LoadFile,
    CleanUp,
    UpdateRenderConfig
} Operation;

ResultCode performOperation(AppContext* context, Params* params, Operation type);

#endif //ENTRYPOINT_H
