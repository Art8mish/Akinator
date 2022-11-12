#ifndef AKINATOR_H_INCLUDED
#define AKINATOR_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <locale.h>
#include <ctype.h>


#include "../onegin_functions/include/onegin_libraries.h"
//#include "../stack_functions/include/libraries.h"
#include "../list_functions/include/ptr_list.h"

#define ASSERT_MODE

#ifdef SOFT_ASSERT
#undef SOFT_ASSERT
#endif

#ifdef ASSERT_MODE

#define SOFT_ASSERT(condition)                                                    \
            do                                                                    \
            {                                                                     \
                if (condition)                                                    \
                    printf("Error in %s = %d; file: %s; num of line: %d \n",      \
                           #condition, condition, __FILE__, __LINE__);            \
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


#define COLOR_TEXT(text) "\e[31m" #text "\e[0m"

#define TREEDUMP(tree, comment)                                      \
            treeDump(tree, comment, __FILE__, __LINE__, __FUNCTION__)

const size_t MAX_STR_SIZE = 101;
const size_t MAX_CMD_SIZE = 4;

typedef const char* tree_elem_t;
static tree_elem_t POISON_TREE_VALUE = "POISON";
#define TREE_SPECIFIER "%s"

#define WRITE_TREE_NODE_VALUE(curr_node, arg)           \
            do                                          \
            {                                           \
                curr_node->value = strdup(arg);         \
            } while (false)

#define FREE_TREE_NODE_VALUE(curr_node)                 \
            do                                          \
            {                                           \
                if (curr_node->value != NULL)           \
                    free((void *)curr_node->value);     \
            } while (false)                         

#define COMPARE_TREE_ELEM(frst_value, scnd_value)       \
            strcmp(frst_value, scnd_value) == 0
            

#define READABLE_SYMB "%[a-zA-z -]"

static const char * const TREE_GRAPH_FILE = "tree_graph.gv";
static const char * const TREE_DUMP_FILE  = "io/tree_dump.txt";
static const char * const TREE_HTM_FILE   = "io/treegraphs.htm"; 
static const char * const TREE_IO         = "io/tree.txt";

enum TreeError
{
    ERROR_TREE_DTOR             = 1,
    ERROR_LIST_DTOR             = 2,
    ERROR_TREE_REMOVE           = 3,
    ERROR_TREE_VERIFICATION     = 4,
    ERROR_TREE                  = 5,
    ERROR_CREATE_NODES          = 6,
    ERROR_SAVE_NODE             = 7,
    ERROR_SYNTAX                = 8,
    ERROR_SSCANF                = 9,
    ERROR_READ_NODE             = 10,
    ERROR_ADD_NEW_CONCEPT       = 11,
    ERROR_GUESS_ITER            = 12,
    ERROR_NULL_TREE_ROOT        = 13,
    ERROR_AKINATOR              = 14,
    ERROR_INCORRECT_ANSWER      = 15,
    ERROR_TREE_INSERT           = 16,
    ERROR_TREE_SAVE             = 17,
    ERROR_NODE_DTOR             = 18,
    ERROR_MAKE_GUESS            = 19,
    ERROR_READ_TREE             = 20,
    ERROR_GREETING              = 21,
    ERROR_GUESS_CONCEPT         = 22,
    ERROR_DEFINITION_ITER_ERROR = 23,
    ERROR_DEFINITION_ITER       = 24,
    ERROR_PRINT_DEFINITION      = 25,
    ERROR_MAKE_DEFINITION       = 26,
    ERROR_FULL_RET_LIST         = 27,
    
};

enum TreeCode
{
    ERROR_CODE_NULL_TREE = (1 << 1),
    CODE_DEAD_TREE = (1 << 30),
};

struct TreeNode
{
    tree_elem_t value = POISON_TREE_VALUE;
    struct TreeNode *left_yes = NULL;
    struct TreeNode *rght_no  = NULL;
    struct TreeNode *prev     = NULL;
};

struct Tree
{
    struct TreeNode *root = NULL;
    unsigned int size     = 0;
    struct List *lst_ret  = NULL;

    int          error    = 0;
};

enum Mode
{
    MODE_GUESS     = 1,
    MODE_DEFINE    = 2,
    MODE_COMPARE   = 3,
    MODE_DUMP      = 4,
    MODE_SAVE_EXIT = 5,
    MODE_EXIT      = 6,
};

int StartAkinator(void);

struct Tree *treeCtor(void);
int treeDtor(struct Tree *tree);
int treeNodeDtor(struct TreeNode *curr_node);

int treeInsert(struct Tree *tree, struct TreeNode *prev_node,
               struct TreeNode **node_ptr, tree_elem_t arg);
    
int treeRemove(struct Tree *tree, struct TreeNode **node_ptr,
               tree_elem_t *arg);
    
int treeVerification(struct Tree *tree);
int treeCheckError(struct Tree *tree);

int treeDump(const struct Tree *tree, const char *comment, 
             const char *file_name, int line_num, const char *func_name);
int CreateTreeGraph(const struct Tree *tree);
int SaveTreeGraphPng(void);
int AddTreeGraphPng(int graph_counter);
int CreateTreeNodes(const struct TreeNode *curr_node, FILE *graph_f);

int treeSave(const struct Tree *tree);
int SaveNode(const struct TreeNode *curr_node, FILE *tree_f);

struct Tree *ReadTree(const char *input_file_name);
int ReadNode(struct Tree *new_tree, struct TreeNode *prev_node,
             struct TreeNode **curr_node, char **buf);

int Greeting(int *mode);

int GuessConcept(struct Tree *tree);
int DoGuessIter(struct Tree *tree, struct TreeNode *curr_node);
int MakeGuess(struct Tree *tree, struct TreeNode *curr_node);

int AddNewConcept(struct Tree *tree, struct TreeNode *curr_node);

int MakeDefinition(struct Tree *tree);
int DoDefinitionIter(struct Tree *tree, struct TreeNode *curr_node,
                     struct TreeNode **desired_node, tree_elem_t concept);
int PrintDefinition(struct TreeNode *curr_node);
int FullRetList(struct TreeNode *curr_node, struct List *ret_list);

#endif //AKINATOR_H_INCLUDED