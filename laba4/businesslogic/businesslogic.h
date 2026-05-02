//
// Created by k3rnel1x on 02.05.2026.
//

#ifndef BUSINESSLOGIC_H
#define BUSINESSLOGIC_H

#include "../app/appcontext.h"
#include "../entrypoint/entrypoint.h"

ResultCode loadFuncTable       (AppContext* context, Params* params);
ResultCode updateRenderConfig  (AppContext* context, Params* params);
ResultCode calcNormalizedCoords(AppContext* context, Params* params);

#endif //BUSINESSLOGIC_H
