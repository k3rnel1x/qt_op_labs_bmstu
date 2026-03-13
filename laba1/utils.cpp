#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <QDebug>

void reverseStr(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
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
    } else if(10 <= n && n <= 35){
        res = '7' + n;
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

char* decToCustom(int dec, int system)
{
    char* res = (char*)calloc(100, sizeof(char));
    char* ptr = res;

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

int* parseToDec(const char* str, int sys)
{
    // char* ptr = strlen(str);
    int neq = (str[0] == '-');
    if(neq) str++;

    int* dec = (int*)calloc(1, sizeof(int));
    int len = strlen(str);
    for(int i = 0, num; i != len; i++)
    {
        num = strToNum(str[i]);
        qDebug() << i << ". dec = " << *dec << " strToNum(str[i]) = " << strToNum(str[i]);
        if(((unsigned)(*dec) + (unsigned)num > 2147483647U))
        {
            free(dec);
            return NULL;
        }

        *dec += pow(sys, len - i - 1)*num;
    }
    if(neq) *dec = ~(*dec) + 1;

    return dec;
}

void removeZeros(char* str)
{
    char* ptr = str;
    while(*ptr == '0')
    {
        for(int i = 0; i < strlen(str); i++)
        {
            str[i] = str[i + 1];
        }
    }
}

int getPowerTwo(int sys)
{
    int res = 0;
    for(int i = 1; !res && i <= 5; i++)
        res = ((int)pow(2, i) == sys)? i : 0;

    return res;
}

char* btwTwoPwr(int decNum, int outPwr) // TODO fix this
{
    char* res = (char*)calloc(200, sizeof(char));
    char* ptr = res;
    size_t count = 32 / outPwr;
    int mask = (int)pow(2, outPwr) - 1;
    qDebug() << "mask = " << mask;
    for (size_t i = 0; i < count; i++)
    {
        int num = (decNum & mask) >> i * outPwr;
        qDebug() << "num = " << num << " decNum = " << decNum << " mask = " << mask << " (decNum & mask) = " << (decNum & mask);
        mask <<= outPwr;
        *ptr++ = numToStr(num);
    }
    reverseStr(res);
    // removeZeros(res);
    // if(decNum < 0) *res = '1';
    return res;
}

int isInCorrectSystem(char* str, char system)
{
    if(*str == '-')
        str++;

    for(int i = 0; str[i] != 0; i++)
        if(!('0' <= str[i] && str[i] < system))
            return 0;
    return 1;
}

int isValidCustomSys(const char* customSys)
{
    int res = 0;
    if(*customSys != 0) {
        int intCustomSys = atoi(customSys);
        if( (1 < intCustomSys) && (intCustomSys < 36) )
            res = 1;
    }
    return res;
}

Result validateData(AppContext* ctx)
{
    // Validate output
    if(ctx->checkedOutputRadioButton == 3 && !isValidCustomSys(ctx->customOutputSystem))
        return OUTPUT_ALPHABET_ERROR;

    qDebug() << "ctx->checkedInputRadioButton = " << ctx->checkedInputRadioButton;
    // Validate input
    Result res = SUCCEED;
    switch(ctx->checkedInputRadioButton)
    {

    case 0:
        res = isInCorrectSystem(ctx->inputText, '2')? SUCCEED : INPUT_ERROR;
        // qDebug() << "isInCorrectSystem() = " << res;
        break;
    case 1:
        res = isInCorrectSystem(ctx->inputText, '8')? SUCCEED : INPUT_ERROR;
        break;
    case 2:
        res = isInCorrectSystem(ctx->inputText, 'A')? SUCCEED : INPUT_ERROR;;
        break;
    case 3:
        if(!isValidCustomSys(ctx->customInputSystem)){
            res = INPUT_ALPHABET_ERROR;
            break;
        }
        int num = atoi(ctx->customInputSystem);
        // qDebug() << "num = " << num;
        char chrNum = numToStr(num);
        // qDebug() << "chrNum = " << chrNum;
        res = isInCorrectSystem(ctx->inputText, chrNum)? SUCCEED : INPUT_ERROR;
        break;
    }
    // qDebug() << "res from validateData() = " << res;


    return res;
}



Result validateInput(AppContext* ctx)
{
    if(!(ctx && ctx->inputText))
        return ERROR;

    Result res = ERROR;
    if( (0 <= ctx->checkedInputRadioButton  && ctx->checkedInputRadioButton  <= 3) &&
        (0 <= ctx->checkedOutputRadioButton && ctx->checkedOutputRadioButton <= 3))
    {
        res = validateData(ctx);
    }

    return res;
}

