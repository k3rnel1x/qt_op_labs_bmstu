#ifndef LOGIC_H
#define LOGIC_H

#include "appcontext.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Result doConvert(AppContext* ctx);
Result doInit   (AppContext* ctx);
Result doDeInit (AppContext* ctx);
Result doClear  (AppContext* ctx);
Result doSwap   (AppContext* ctx);

void swapPtr(char** ptr1, char** ptr2);

#endif // LOGIC_H
