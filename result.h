#ifndef RESULT_H
#define RESULT_H

typedef enum result_code {
    SUCCESS,
    ERROR,
    FILE_EMPTY,
    TABLE_EMPTY,
    NO_FILE,
    NAME_COLLUM,
    INVALID_COLLUM,
    OUTRANGE_COLLUM,
    EMPTY_COLLUM,
    EMPTY_REGION,
    NO_REGION
} result_code;

#endif // RESULT_H
