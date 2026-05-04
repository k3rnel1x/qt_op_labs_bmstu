//
// Created by k3rnel1x on 02.05.2026.
//

#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <cstddef>

typedef struct AppContext {
    const char* filenamePath;
    size_t maxStep;
    size_t minStep;
    size_t currStep;
    size_t maxNormalizationRange;
} AppContext;

typedef enum ResultCode {
    SUCCEED,
    ERROR,
    WRONG_FILE_EXTENSION,
    NO_FILE,
    EMPTY_FILE
} ResultCode;

typedef struct Params {
    const char* filenamePath;
} Params;

#endif //APPCONTEXT_H
