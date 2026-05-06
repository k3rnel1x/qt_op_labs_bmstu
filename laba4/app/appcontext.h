//
// Created by k3rnel1x on 02.05.2026.
//

#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <cstddef>
#include "../businesslogic/pointsarr/pointsarr.h"

typedef struct AppContext {
    // UI between Backend
    const char* filenamePath;
    size_t renderStep;
    size_t maxNormalizationRange;
    size_t minNormalizationRange;
    PointsArr points;

    // to UI
    size_t maxStep;
    size_t minStep;
} AppContext;

typedef enum ResultCode {
    SUCCEED,
    ERROR,
    WRONG_FILE_EXTENSION,
    NO_FILE,
    EMPTY_FILE,
    INVALIDTABLE
} ResultCode;

typedef struct Params {
    const char* filenamePath;
    size_t renderStep;
    size_t maxNormalizationRange;
    size_t minNormalizationRange;
} Params;

#endif //APPCONTEXT_H
