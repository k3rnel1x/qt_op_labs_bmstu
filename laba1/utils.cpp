#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <QDebug>

#define INVERT_CHAR(ch, base) numToStr((base) - 1 - strToNum((ch)))
#define MAX_INT32 2147483647
#define MIN_INT32 -2147483648

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

char* cnvDecToBase(int dec, int base)
{
    char* buff = (char*)calloc(100, sizeof(char));
    char chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i = 0;
    unsigned int num = (unsigned int)dec;
    if(base == 10)
    {
        sprintf(buff, "%d", dec);
        return buff;
    }
    qDebug() << "num = " << num;


    if (num == 0) {
        strcpy(buff, "0");
        return buff;
    }

    while (num > 0) {
        buff[i++] = chars[num % base];
        num /= base;
    }

    reverseStr(buff);
    return buff;
}

Result cnvFromBaseToDec(const char* target, int target_base, int* dec)
{
    if(!target || !dec || target_base <= 1) return ERROR;

    int maxLen = strlen(cnvDecToBase(*dec, target_base));
    qDebug() << "maxLen = " << maxLen;

    long int ldec = strtol(target, NULL, target_base);
    qDebug() << "ldec = " << ldec;
    if(strlen(target) == maxLen){
        if(2147483647L <= ldec && ldec <= 4294967295L)
        {
            *dec = (int)ldec;
            qDebug() << "dec = " << *dec;
            return SUCCEED;
        }
    } else {
        if(-2147483648L <= ldec && ldec <= 2147483647L)
        {
            *dec = (int)ldec;
            // qDebug() << "dec = " << *dec;
            return SUCCEED;
        }
    }

    return TOO_LARGE_NUM_ERROR;
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


