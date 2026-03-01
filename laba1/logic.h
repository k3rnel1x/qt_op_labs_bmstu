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
int  strToNum(char c);
char numToStr(int n);
void reverseStr(char* str);
char* decToCustom(int dec, int system);
int parseToDec(const char* str, int sys);
int  getSystemById(AppContext* ctx , int is_output);
int getPowerTwo(int sys);
char* btwTwoPwr(int decNum, int outPwr);

#endif // LOGIC_H
