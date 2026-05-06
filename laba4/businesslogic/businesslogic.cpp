
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "businesslogic.h"
#include "../app/logger.hpp"
#include "./pointsarr/pointsarr.h"
#include "./parser/parser.h"

#define DIMENSIONSIZE 1000

int checkFileExtension(const char* path);
void clearFilename(AppContext* context);
void clearPoints(AppContext* context);

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
    PointsArr arr = getPointsArr();
    size_t matrixSize = 0;
    
    ResultCode res = parceCSVpoints(f, &arr, &matrixSize);
    if(res == SUCCEED)
    {
        clearFilename(context);
        context->filenamePath = path;
        params->filenamePath  = NULL;

        clearPoints(context);
        context->points = arr;

        context->minStep = 1;
        context->maxStep = DIMENSIONSIZE / matrixSize;
    }
    
    fclose(f);
    return res;
}

ResultCode updateRenderConfig(AppContext* context, Params* params)
{
    if(!context || !params) return ERROR;
    ResultCode result = SUCCEED;

    // insert parameters to context
    context->maxNormalizationRange = params->maxNormalizationRange;    
    context->minNormalizationRange = params->minNormalizationRange;
    context->renderStep = params->renderStep;    
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
    clearPoints(context); 

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

void clearPoints(AppContext* context)
{
    Logger::get_instance().logDebug("Clearing points..");

    if(!context) return;
    if(!context->points.points) return;

    delPointsArr(&context->points);
}

// utils

int checkFileExtension(const char* path)
{
    return !strcmp(path + strlen(path) - strlen(".csv") , ".csv");
}