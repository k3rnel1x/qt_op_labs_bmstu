#ifndef RESULT_H
#define RESULT_H

typedef enum Result {
    SUCCESS,
    RUNTIME_ERROR,
    
    NO_FILE, // logic error
    EMPTY_FILE,
    INVALID_HEADER
} Result;

#endif // RESULT_H
