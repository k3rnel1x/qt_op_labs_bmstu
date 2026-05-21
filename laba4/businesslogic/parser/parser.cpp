#include "parser.h"
#include <iostream>
using namespace std;

void fillNeibors(PointsArr* arr, size_t matrix_size);

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
        while(*ptr)
        {

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

    fillNeibors(arr, *matrixSize);
    // for (int i = 0; i < arr->count; ++i)
    //     printf("Point: (%02lf %02lf %02lf) (neibors: %p %p %p %p)\n",
    //         arr->points[i].x,
    //         arr->points[i].y, 
    //         arr->points[i].z,

    //         arr->points[i].neibors[0],
    //         arr->points[i].neibors[1],
    //         arr->points[i].neibors[2],
    //         arr->points[i].neibors[3]
    //     );
    return result;
}

void fillNeibors(PointsArr* arr, size_t matrix_size)
{
    for(int i = 0; i < arr->count; ++i)
    {
        memset(arr->points[i].neibors, NULL, sizeof(Point*)*4);

        if(i - matrix_size >= 0){
            arr->points[i].neibors[0] = arr->points + i - matrix_size;
        }

        if(i + matrix_size < arr->count){
            arr->points[i].neibors[2] = arr->points + i + matrix_size;
        }

        if (i - 1 >= 0){
            arr->points[i].neibors[1] = arr->points + i - 1;
        }

        if (i + 1 < arr->count){
            arr->points[i].neibors[3] = arr->points + i + 1;
        }
    }
}
