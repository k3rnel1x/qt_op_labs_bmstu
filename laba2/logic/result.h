#ifndef RESULT_H
#define RESULT_H

typedef enum Result {
    SUCCESS,
    RUNTIME_ERROR,

    NO_FILE_SELECTED, // ui error
    NO_FILE, // logic error
    EMPTY_FILE,
    INVALID_HEADER
} Result;

#endif // RESULT_H
