#ifndef APPCONTEXT_H
#define APPCONTEXT_H
#include <string>

enum NumSystem {
    TWO=2,
    TEN=10,
    SXTEEN=16
};

typedef struct AppContext {
    char* inputText;
    NumSystem iptsys;
    char* outputText;
    NumSystem outsys;
} AppContext;

#endif // APPCONTEXT_H
