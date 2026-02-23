#include "logic.h"
#include "appcontext.h"

void doConvert(AppContext* ctx)
{

}


void doInit(AppContext* ctx)
{
    ctx->checkedInputRadioButton  = 2; // 10
    ctx->checkedOutputRadioButton = 0; // 2
}

void doDeInit(AppContext* ctx)
{

}

void doClear(AppContext* ctx)
{
    if(ctx->inputText) {
        free(ctx->inputText);
        ctx->inputText = NULL;
    }

    if(ctx->outputText) {
        free(ctx->outputText);
        ctx->outputText = NULL;
    }
}

