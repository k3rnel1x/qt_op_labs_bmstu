#include "logic.h"
#include "appcontext.h"
#include "entrypoint.h"
#include <string.h>

Result doConvert(AppContext* ctx)
{
    Result res = SUCCEED;
    char* outStr = (char*)calloc(strlen(ctx->inputText)+1, sizeof(char));
    strcpy(outStr, ctx->inputText);
    ctx->outputText = outStr;
    return res;
}


Result doInit(AppContext* ctx)
{
    Result res = SUCCEED;
    ctx->checkedInputRadioButton  = 2; // 10
    ctx->checkedOutputRadioButton = 0; // 2
    return res;
}

Result doDeInit(AppContext* ctx)
{
    Result res = SUCCEED;
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

Result doSwap(AppContext *ctx)
{
    Result res = SUCCEED;
    return res;
}
