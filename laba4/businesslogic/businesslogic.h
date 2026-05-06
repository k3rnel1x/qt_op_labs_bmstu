//
// Created by k3rnel1x on 02.05.2026.
//

#ifndef BUSINESSLOGIC_H
#define BUSINESSLOGIC_H

#include "../app/appcontext.h"
#include "./parser/parser.h"

ResultCode loadFuncTable       (AppContext* context, Params* params);
ResultCode updateRangeConfig   (AppContext* context, Params* params);
ResultCode updateStepConfig    (AppContext* context, Params* params);
ResultCode calcNormalizedCoords(AppContext* context);
ResultCode deleteContext       (AppContext* context);

#endif //BUSINESSLOGIC_H