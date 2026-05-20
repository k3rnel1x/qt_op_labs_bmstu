#include "parser.h"
#include <iostream>
using namespace std;

void fillNeibors(PointsArr* arr);

ResultCode parceCSVpoints(FILE* f, PointsArr* arr, size_t* matrixSize)
{
    if(!f  || feof(f) || getc(f) == EOF || !matrixSize) return ERROR;
    rewind(f);
    if(!arr || !arr->points) return ERROR;

    ResultCode result = SUCCEED;


    char buff[BUFFERSIZE] = {0};

    double row = 0;
    char* res;
    do {
        res = fgets(buff, BUFFERSIZE, f);
        if(!res)
            break;

        char* ptr = buff;
        char* endPtr;
        double col = 0;
        while(*ptr){

            double val = strtod(ptr, &endPtr);
            if(endPtr == ptr){
                delPointsArr(arr);
                result = INVALIDTABLE;
                break;
            }

            Point point = {
                .x = col,
                .y = row,
                .z = val,
            };

            addPoint(arr, point);
            ptr = endPtr + 1;
            ++col;
        }

        ++row;
    } while(result == SUCCEED);

    if(result == SUCCEED)
    {
        *matrixSize = row;
    }

    fillNeibors(arr);

    return result;
}

void fillNeibors(PointsArr* arr)
{
    // TODO
}
