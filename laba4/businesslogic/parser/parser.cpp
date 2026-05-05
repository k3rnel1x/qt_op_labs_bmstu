#include "parser.h"

ResultCode parceCSVpoints(FILE* f, PointsArr* arr, size_t* matrixSize)
{
    if(!f  || feof(f) || getc(f) == EOF || !matrixSize) return ERROR;
    rewind(f);
    if(!arr || !arr->points) return ERROR;

    ResultCode result = SUCCEED;


    char buff[BUFFERSIZE] = {0};

    size_t row = 0;
    char* res;
    do {
        res = fgets(buff, BUFFERSIZE, f);
        if(!res)
            break;

        char* ptr = buff;
        char* endPtr;
        while(*ptr){
            size_t col = 0;

            long val = strtol(ptr, &endPtr, BASE);
            if(!endPtr){
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

    return result;
}