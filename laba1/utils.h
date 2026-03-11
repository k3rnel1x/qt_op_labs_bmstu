#ifndef UTILS_H
#define UTILS_H
#include "appcontext.h"
#include "errors.h"

Result validateInput(AppContext* ctx);
Result validateData(AppContext* ctx);
int isValidCustomSys(const char* customSys);
int isInCorrectSystem(char* str, char system);

void  swapPtr(char** ptr1, char** ptr2);
int   strToNum(char c);
char  numToStr(int n);
void  reverseStr(char* str);
char* decToCustom(int dec, int system);
int   parseToDec(const char* str, int sys);
int   getSystemById(AppContext* ctx , int is_output);
int   getPowerTwo(int sys);
char* btwTwoPwr(int decNum, int outPwr);
void  removeZeros(char* str);

#endif // UTILS_H
