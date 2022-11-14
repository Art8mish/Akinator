#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

enum Error
{
    ERROR_TREE_DTOR             = 1,
    ERROR_LIST_DTOR             = 2,
    ERROR_TREE_REMOVE           = 3,
    ERROR_SSCANF                = 9,
    ERROR_ADD_NEW_CONCEPT       = 11,
    ERROR_GUESS_ITER            = 12,
    ERROR_NULL_TREE_ROOT        = 13,
    ERROR_AKINATOR              = 14,
    ERROR_INCORRECT_ANSWER      = 15,
    ERROR_TREE_SAVE             = 17,
    ERROR_MAKE_GUESS            = 19,
    ERROR_READ_TREE             = 20,
    ERROR_GREETING              = 21,
    ERROR_GUESS_CONCEPT         = 22,
    ERROR_DEFINITION_ITER_ERROR = 23,
    ERROR_DEFINITION_ITER       = 24,
    ERROR_PRINT_DEFINITION      = 25,
    ERROR_MAKE_DEFINITION       = 26,
    ERROR_FULL_RET_LIST         = 27,
    ERROR_AKINATOR_CTOR         = 28,
    ERROR_AKINATOR_DTOR         = 29,
    ERROR_PRINT_COMPARISON      = 30,
    ERROR_PRINT_LIST_RET        = 31,
    ERROR_COMPARE_CONCEPTS      = 32,
};

#endif //ERROR_H_INCLUDED