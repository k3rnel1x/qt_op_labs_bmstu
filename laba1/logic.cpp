#include "logic.h"
#include "appcontext.h"
#include <string.h>
#include <math.h>
#include "utils.h"

Result doConvert(AppContext* ctx)
{
    // Result res = SUCCEED;
    Result res = validateInput(ctx);
    if(res == SUCCEED)
    {

        int inputSys  = getSystemById(ctx, 0);
        int outputSys = getSystemById(ctx, 1);
        int pwrTwo    = getPowerTwo(outputSys);

        int decNum = parseToDec(ctx->inputText, inputSys);
        char* outStr = NULL;
        if(pwrTwo)
        {
            outStr = btwTwoPwr(decNum, pwrTwo);
        } else {
            outStr = decToCustom(decNum, outputSys);
        }
        // printf("decNum = %d\n", decNum);
        ctx->outputText = outStr;
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
    swapPtr(&ctx->customInputSystem, &ctx->customOutputSystem);
    return res;
}

void swapPtr(char** ptr1, char** ptr2)
{
    char* tmp = *ptr2;
    *ptr2 = *ptr1;
    *ptr1 = tmp;
}
