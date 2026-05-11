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

ResultCode updateRangeConfig(AppContext* context, Params* params)
{
    if(!context || !params) return ERROR;
    ResultCode result = SUCCEED;

    // insert parameters to context
    context->maxNormalizationRange = params->maxNormalizationRange;    
    context->minNormalizationRange = params->minNormalizationRange;
    return result;
}

ResultCode updateStepConfig(AppContext* context, Params* params)
{
    if(!context || !params) return ERROR;
    ResultCode result = SUCCEED;

    // insert parameters to context
    context->renderStep = params->renderStep;    
    return result;
}

ResultCode calcNormalizedCoords(AppContext* context)
{
    if(!context || !context->points.points) return ERROR;
    if(context->minNormalizationRange == context->maxNormalizationRange) return RANGESAREINDENTICAL;

    ResultCode result = SUCCEED;

    size_t minInRange = context->minNormalizationRange;
    size_t maxInRange = context->maxNormalizationRange;

    PointsArr* arr = &context->points;

    double zMin = arr->points[0].z;
    double zMax = arr->points[0].z;
    for(int i = 0; i < arr->count; ++i)
    {
        double z = arr->points[i].z;

        if(zMax < z)
            zMax = z;

        if(zMin > z)
            zMin = z;
    }

    // if(zMin == minInRange && zMax == maxInRange){
        // context->normPoints = context->points;
        // return result;
    // }

    char logText[100] = {0};
    sprintf(logText, "starting normalize.. Params: Norm range: [%zu, %zu] zRange: [%lf, %lf]", minInRange, maxInRange, zMin, zMax);
    Logger::get_instance().logDebug(logText);
    qDebug() << logText;

    PointsArr normArr = getPointsArr();
    for(int i = 0; i < arr->count; ++i)
    {
        double z = arr->points[i].z;
        double normZ = minInRange + double(z - zMin) / double(zMax - zMin) * double(maxInRange - minInRange);
        Point p = {
            .x = arr->points[i].x,
            .y = arr->points[i].y,
            .z = normZ,
        };
        addPoint(&normArr, p);
        // qDebug("|zold = %lf|znew = %lf|\n", z, arr->points[i].z);
    }
    Logger::get_instance().logDebug("normalizing succeed.");

    // push to context normPoints
    context->normPoints = normArr;
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