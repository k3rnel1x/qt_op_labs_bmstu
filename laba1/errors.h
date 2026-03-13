#ifndef ERRORS_H
#define ERRORS_H

typedef enum Result {
    SUCCEED,
    ERROR,
    INPUT_ERROR,
    NUM_RANGE_ERROR,
    INPUT_ALPHABET_ERROR,
    OUTPUT_ALPHABET_ERROR,
    TOO_LARGE_NUM_ERROR
} Result;

#endif // ERRORS_H
