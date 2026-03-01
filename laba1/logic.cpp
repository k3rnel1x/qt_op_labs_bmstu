#include "logic.h"
#include "appcontext.h"
#include "entrypoint.h"
#include <string.h>
#include <math.h>
#include <bitset>
#include <iostream>

// #include <limits.h> // for CHAR_BIT

// void print_binary(int num) {
//     // Calculate the number of bits in an unsigned int
//     int num_bits = sizeof(int) * CHAR_BIT;

//     // Iterate from the most significant bit down to the least significant bit
//     for (int i = num_bits - 1; i >= 0; i--) {
//         // Use bitwise shift (>>) and bitwise AND (&) to check each bit
//         int bit = (num >> i) & 1;
//         qDebug("%u", bit);
//     }
//     qDebug("\n");
// }

// TODO implement error handlers
Result doConvert(AppContext* ctx)
{
    // qDebug() << "Info:\n" << ctx->checkedInputRadioButton << ctx->checkedOutputRadioButton
             // << '\n' << ctx->inputText << ctx->outputText;
    // Result res = validateInput(ctx);

    Result res = SUCCEED;
    if(res == SUCCEED)
    {

        int inputSys  = getSystemById(ctx, 0);
        int outputSys = getSystemById(ctx, 1);
        int pwrTwo    = getPowerTwo(inputSys);

        int decNum = parseToDec(ctx->inputText, inputSys);
        char* outStr = NULL;
        if(pwrTwo && pwrTwo != 2)
        {
            outStr = btwTwoPwr(decNum, pwrTwo);
        } else {
            outStr = decToCustom(decNum, outputSys);
        }
        ctx->outputText = outStr;

        // qDebug() << "inputSys: " << inputSys << "outputSys: " << outputSys;
        // qDebug() << "getSystemById(ctx, 1): " << getSystemById(ctx, 1);
        // print_binary(decNum);

    }

    return res;
}


Result doInit(AppContext* ctx)
{
    Result res = SUCCEED;
    ctx->checkedInputRadioButton  = 2; // 10
    ctx->checkedOutputRadioButton = 0; // 2

    // Init text fields
    ctx->inputText = (char*)calloc(2, sizeof(char));
    ctx->inputText[0] = '0';

    ctx->outputText = (char*)calloc(2, sizeof(char));
    ctx->outputText[0] = '0';

    return res;
}

Result doDeInit(AppContext* ctx)
{
    Result res = SUCCEED;
    if(ctx->inputText != NULL)
        free(ctx->inputText);

    if(ctx->outputText != NULL)
        free(ctx->outputText);
    return res;
}

Result doClear(AppContext* ctx)
{
    Result res = SUCCEED;
    if(ctx->inputText) {
        free(ctx->inputText);
        ctx->inputText = NULL;
    }

    if(ctx->customInputSystem) {
        free(ctx->customInputSystem);
        ctx->customInputSystem = NULL;
    }


    if(ctx->outputText) {
        free(ctx->outputText);
        ctx->outputText = NULL;
    }

    if(ctx->customOutputSystem) {
        free(ctx->customOutputSystem);
        ctx->customOutputSystem = NULL;
    }
    return res;
}

Result doSwap(AppContext* ctx)
{
    Result res = SUCCEED;
    // Swap int's
    int* input = &ctx->checkedInputRadioButton;
    int* output = &ctx->checkedOutputRadioButton;
    if(*input != *output)
    {
        int tmp = *output;
        *output = *input;
        *input = tmp;
    }


    // Swap ptr's
    swapPtr(&ctx->inputText, &ctx->outputText);
    // qDebug() << ctx->inputText << ctx->outputText;
    swapPtr(&ctx->customInputSystem, &ctx->customOutputSystem);
    return res;
}

void swapPtr(char** ptr1, char** ptr2)
{
    char* tmp = *ptr2;
    *ptr2 = *ptr1;
    *ptr1 = tmp;
}

int strToNum(char c)
{
    int res = 0;
    if('0' <= c && c <= '9')
    {
        res = c - 48;
    } else if('A' <= c && c <= 'Z'){
        res = 10 + c - 'A';
    }

    return res;
}

char numToStr(int n)
{
    char res = 0;
    if(0 <= n && n <= 9)
    {
        res = '0' + n;
    } else if(10 <= n && n <= 36){
        res = 'A' + n - 10;
    }

    return res;
}

int getSystemById(AppContext* ctx, int is_output)
{
    int sys;
    int id = is_output? ctx->checkedOutputRadioButton : ctx->checkedInputRadioButton;
    switch (id) {
    case 0:
        sys = 2;
        break;
    case 1:
        sys = 8;
        break;
    case 2:
        sys = 10;
        break;
    case 3:
        sys = atoi(is_output? ctx->customOutputSystem : ctx->customInputSystem);
        break;
    default:
        sys = -1;
    }
    return sys;
}

// 1234
char* decToCustom(int dec, int system)
{
    char* res = (char*)calloc(33, sizeof(char));
    char* ptr = res;
    // int is_neq = 0;
    // if(dec < 0){
    //     is_neq = 1;
    //     dec = -dec;
    // }

    for(int i = 0; dec != 0; i++)
    {
        *ptr++ = numToStr(dec % system);
        dec /= system;
    }
    reverseStr(res);
    return res;
}

int getIntLen(int n)
{
    int len = 0;
    for(int k = n, p = 1; k != 0;)
    {
        len++;
        k /= 10*p;
    }
    return len;
}

void reverseStr(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int parseToDec(const char* str, int sys)
{
    // char* ptr = strlen(str);
    int neq = (str[0] == '-');
    if(neq) str++;

    int dec = 0;
    int len = strlen(str);
    for(int i = len - 1; i != -1; i--)
        dec += pow(sys, len - i - 1)*strToNum(str[i]);
    if(neq) dec = ~dec + 1;

    return dec;
}

int getPowerTwo(int sys)
{
    int res = 0;
    for(int i = 1; !res && i <= 5; i++)
        res = ((int)pow(2, i) == sys)? i : 0;

    return res;
}

typedef struct HalfByte {
    char b : 4;
} HalfByte;

char* btwTwoPwr(int decNum, int outPwr)
{
    char* res = (char*)calloc(33, sizeof(char));
    char* ptr = res;
    char* dp = (char*)&decNum;
    for(int i = 0; i < 9; i++)
    {
        HalfByte hbyte = {.b=*dp};
        *ptr++ = numToStr(hbyte.b);
        dp++;
    }
    reverseStr(res);

    return res;
}

