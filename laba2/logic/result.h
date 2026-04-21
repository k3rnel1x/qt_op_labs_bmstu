#ifndef RESULT_H
#define RESULT_H

typedef enum Result {
    SUCCESS,
    RUNTIME_ERROR,
    NO_CSV_FILE,

    // logic error
    NO_FILE,
    EMPTY_FILE,
    INVALID_HEADER,

    // calc error
    NOT_ENOUGH_INFO,
} Result;

#endif // RESULT_H
