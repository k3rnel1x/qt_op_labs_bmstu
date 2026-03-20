#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <QDebug>
#include "validate.h"

Result validateCustomInputSystem(AppContext* ctx)
{
    if(!ctx || (ctx->checkedInputRadioButton == 3 && !ctx->customInputSystem)) return INPUT_ALPHABET_ERROR;
    Result res = SUCCEED;
    if(ctx->checkedInputRadioButton == 3)
    {
        char* sys = ctx->customInputSystem;
        for(int i = 0; res == SUCCEED && i < strlen(sys); i++)
            res = ('0' <= sys[i] && sys[i] <= '9')? SUCCEED : INPUT_ALPHABET_ERROR;

        if(res == SUCCEED)
            res = 1 < atoi(sys) && atoi(sys) < 37? SUCCEED : INPUT_ALPHABET_ERROR;
    }
    return res;
}


Result validateCustomOutputSystem(AppContext* ctx)
{
    if(!ctx || (ctx->checkedOutputRadioButton == 3 && !ctx->customOutputSystem)) return OUTPUT_ALPHABET_ERROR;
    Result res = SUCCEED;
    if(ctx->checkedOutputRadioButton == 3)
    {
        char* sys = ctx->customOutputSystem;
        for(int i = 0; res == SUCCEED && i < strlen(sys); i++)
            res = ('0' <= sys[i] && sys[i] <= '9')? SUCCEED : OUTPUT_ALPHABET_ERROR;

        if(res == SUCCEED)
            res = 1 < atoi(sys) && atoi(sys) < 37? SUCCEED : INPUT_ALPHABET_ERROR;
    }
    return res;
}

int validChar(char c, char sys)
{
    if('0' <= sys && sys <= '9')
        return '0' <= c && c < sys;
    else if('a' <= sys && sys < 'z')
        return ('0' <= c && c <= '9') || ('a' <= c && c < sys);
    else
        return ('0' <= c && c <= '9') || ('A' <= c && c < sys);
}

Result validateInputText(AppContext* ctx)
{
    if(!(ctx && ctx->inputText))
        return ERROR;
    Result res = SUCCEED;
    char sys;
    switch(ctx->checkedInputRadioButton) {
    case 0: sys = '2'; break;
    case 1: sys = '8'; break;
    case 2: sys = 'A'; break;
    case 3: sys = numToStr(atoi(ctx->customInputSystem)); break;}
    // qDebug() << "sys = " << sys << " "
    char* text = ctx->inputText;
    if(text[0] == ' ')
        res = INPUT_ERROR;

    if(res == SUCCEED && (getSystemById(ctx, 0) != 10 && *text == '-'))
        res = INPUT_ERROR;

    if(res == SUCCEED && getSystemById(ctx, 0) == 10 && *text == '-')
        text++;

    if(res == SUCCEED)
    {
        for(int i = 0; res == SUCCEED && i < strlen(text); i++)
        {
            char ch = text[i];
            if('a' <= ch && ch <= 'z')
                ch = 64 + ch - 96;
            res = validChar(ch, sys)? SUCCEED : INPUT_ERROR;
        }
    }

    return res;
}
