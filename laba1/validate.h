#ifndef VALIDATE_H
#define VALIDATE_H
#include "utils.h"

int isInCorrectSystem(char* str, char system);
int isValidCustomSys(const char* customSys);
Result validateData(AppContext* ctx);
Result validateCustomOutputSystem(AppContext* ctx);
Result validateCustomInputSystem(AppContext* ctx);
Result validateInputText(AppContext* ctx);

#endif // VALIDATE_H
