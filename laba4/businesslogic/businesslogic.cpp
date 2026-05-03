#include "businesslogic.h"
#include "../app/logger.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int checkFileExtension(const char* path);
void clearFilename(AppContext* context);

ResultCode loadFuncTable(AppContext* context, Params* params)
{
    if(!context || !params) return ERROR;

    const char* path = params->filenamePath;
    // validations
    int is_csv = checkFileExtension(path);
    if(!is_csv) return WRONG_FILE_EXTENSION;

    FILE *f = fopen(path, "r");
    if (!f) return NO_FILE;
    if (feof(f) || getc(f) == EOF) return EMPTY_FILE;
    rewind(f);

    // main part

    
    // TODO load and parce table

    
    fclose(f);
    // push to context
    clearFilename(context);
    context->filenamePath = path;
    params->filenamePath  = NULL;

    return SUCCEED;
}

ResultCode updateRenderConfig(AppContext* context, Params* params)
{
    if(!context || !params) return ERROR;

    ResultCode result = SUCCEED;
    return result;
}

ResultCode calcNormalizedCoords(AppContext* context, Params* params)
{
    if(!context || !params) return ERROR;

    ResultCode result = SUCCEED;
    return result;
}

ResultCode deleteContext(AppContext* context)
{
    Logger::get_instance().logDebug("->Clear context..");
    if(!context) return SUCCEED;

    clearFilename(context);
    // TODO clearPoints(); 

    Logger::get_instance().logDebug("->Clear context succeed");
    return SUCCEED;
}

void clearFilename(AppContext* context)
{
    Logger::get_instance().logDebug("Clearing filename..");

    if(!context) return;
    if(!context->filenamePath) return;

    free((char*)context->filenamePath);
    context->filenamePath = NULL;
}

// utils

int checkFileExtension(const char* path)
{
    return !strcmp(path + strlen(path) - strlen(".csv") , ".csv");
}