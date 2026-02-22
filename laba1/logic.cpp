#include "logic.h"
#include "appcontext.h"
#include <stdio.h>
#include <stdlib.h>

void convert(AppContext* ctx)
{

}


void initialize(AppContext* ctx)
{
	ctx->inputText =  (char*)calloc(500, 1);
	ctx->outputText = (char*)calloc(500, 1);
	ctx->iptsys = TEN;	
	ctx->outsys = TWO;	
}

void deinitialize(AppContext* ctx)
{
	free(ctx->inputText);
	free(ctx->outputText);
}

