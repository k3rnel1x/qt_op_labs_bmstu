#ifndef PARSER
#define PARSER

#include <stdio.h>
#include "../../app/appcontext.h"
#include "../pointsarr/pointsarr.h"

#define BUFFERSIZE 100

ResultCode parceCSVpoints(FILE* f, PointsArr* arr, size_t* matrixSize);

#endif