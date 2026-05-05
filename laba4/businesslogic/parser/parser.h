#ifndef PARSER
#define PARSER

#include <stdio.h>
#include <appcontext.h>
#include <pointsarr.h>

#define BUFFERSIZE 100
#define BASE 10

ResultCode parceCSVpoints(FILE* f, PointsArr* arr, size_t* matrixSize);

#endif