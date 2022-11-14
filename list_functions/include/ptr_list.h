#ifndef PTR_LIST_H_INCLUDED
#define PTR_LIST_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#include "dump.h"

#define ASSERT_MODE

#ifdef SOFT_ASSERT
#undef SOFT_ASSERT
#endif

#ifdef ASSERT_MODE
#define SOFT_ASSERT(condition)                                                    \
            do                                                                    \
            {                                                                     \
                if (condition)                                                    \
                    printf("Error in %s; file: %s; num of line: %d \n",           \
                           #condition, __FILE__, __LINE__);                       \
            } while(false)

#else
#define SOFT_ASSERT(condition) ;
#endif

#define ERROR_CHECK(cond, error)                            \
            do                                              \
            {                                               \
                SOFT_ASSERT(cond);                          \
                if (cond)                                   \
                    return error;                           \
            } while(false)

#define FILE_ERROR_CHECK(cond, error, closing_file)         \
            do                                              \
            {                                               \
                SOFT_ASSERT(cond);                          \
                if (cond)                                   \
                {                                           \
                    fclose(closing_file);                   \
                    return error;                           \
                }                                           \
            } while(false)

#define LISTDUMP(list, comment)                                      \
            listDump(list, comment, __FILE__, __LINE__, __FUNCTION__)

typedef struct TreeNode * list_elem_t;
#define LIST_SPECIFIER "%-6d"

static struct TreeNode *LIST_POISON_VALUE = NULL;

static const char *const GRAPH_FILE = "list_functions/graph.gv";
static const char *const  DUMP_FILE = "list_functions/io/dump_file.txt";
static const char *const   HTM_FILE = "list_functions/io/list_graphs.htm"; 

enum ListError
{
    SUCCESS                   = 0,
    ERROR_CTOR                = 1,
    ERROR_DTOR                = 2,
    ERROR_CALLOC              = 3,
    ERROR_NULL_PTR            = 4,
    ERROR_NULL_TAIL           = 5,
    ERROR_NULL_FREE           = 6,
    ERROR_VERIFICATION        = 7,
    ERROR_INITIALIZE_NODES    = 8,
    ERROR_OPENING_FILE        = 9,
    ERROR_CLOSING_FILE        = 10,
    ERROR_PUSH_BACK           = 11,
    ERROR_PUSH_FRONT          = 12,
    ERROR_POP_BACK            = 13,
    ERROR_POP_FRONT           = 14,
    ERROR_CREATE_GRAPH        = 15,  
    ERROR_ADD_GRAPH           = 16,
    ERROR_NULL_INDX           = 17,
    ERROR_INCORRECT_OPERATION = 18,
    ERROR_LIST_CHECK_ERROR    = 19,
    ERROR_REALLOC             = 20,
    ERROR_LIST_RESIZE         = 21,
    ERROR_LIST_INSERT_AFTER   = 22,
    ERROR_LIST_INSERT_BEFORE  = 23,
    ERROR_LIST_REMOVE         = 24,
};

enum ListErrorCode 
{
    ERROR_CODE_NULL_HEAD          = (1 << 1),
    ERROR_CODE_NULL_TAIL          = (1 << 2),
    ERROR_CODE_HEAD_IS_TAIL       = (1 << 3),
    ERROR_CODE_NULL_FICT_NODE     = (1 << 4),
    ERROR_OPER_WITH_DEAD_LIST     = (1 << 5),
    CODE_DEAD_LIST                = (1 << 30),
};


struct Node
{
    list_elem_t value = LIST_POISON_VALUE;
    struct Node *next = 0;
    struct Node *prev = 0;
};

struct List
{
    struct Node *fict_node = NULL;
    size_t size            = 0;
    int    error           = 0;
};

int listCtor(struct List *list);
int listDtor(struct List *list);

int listPushBack (struct List *list, list_elem_t arg,
                  struct Node **new_node_ptr);
int listPushFront(struct List *list, list_elem_t arg,
                  struct Node **new_node_ptr);

int listPopBack (struct List *list, list_elem_t *arg);
int listPopFront(struct List *list, list_elem_t *arg);

int listInsertAfter(struct List *list, struct Node *node_ptr,
                      list_elem_t arg, struct Node **new_node_ptr);
int listInsertBefore(struct List *list, struct Node *node_ptr,
                       list_elem_t arg, struct Node **new_node_ptr);

int listRemove(struct List *list, struct Node *node_ptr,
               list_elem_t *arg);

int list_Translate_Logical_Position_To_Physical_Adress(struct List *list,
                                                       unsigned int logic_indx, 
                                                       struct Node **phys_indx);

int listVerification(struct List *list);
int listCheckError(struct List *list);

#endif // PTR_LIST_H_INCLUDED