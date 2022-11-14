#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <locale.h>
#include <ctype.h>

#include "../onegin_functions/include/onegin_libraries.h"
#include "common.h"

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

#define TREEDUMP(tree, comment)                                              \
            TreeDump(tree, comment, true, __FILE__, __LINE__, __FUNCTION__)


typedef const char *tree_elem_t;
static tree_elem_t POISON_TREE_VALUE = "POISON";
#define TREE_SPECIFIER "%s"

#define READABLE_SYMB "%[a-zA-Z1234567890 -]"

static const char *const         TREE_GRAPH_PATH = "tree_graph.gv";
static const char *const          TREE_DUMP_PATH = "io/tree_dump.txt";
static const char *const           TREE_HTM_PATH = "io/tree_graphs.htm"; 
static const char *const TREE_SERIALIZATION_PATH = "io/tree.txt";

const size_t MAX_TREE_STR_SIZE = 101;
const size_t MAX_COMMAND_SIZE = 101;

#ifndef PTR_LIST_H_INCLUDED
enum ListError
{
    SUCCESS                   = 0,
    ERROR_CALLOC              = 3,
    ERROR_NULL_PTR            = 4,
    ERROR_OPENING_FILE        = 9,
    ERROR_CLOSING_FILE        = 10,
    ERROR_CREATE_GRAPH        = 15,  
    ERROR_ADD_GRAPH           = 16,
};
#endif

enum TreeError
{
    ERROR_TREE                     = 1,
    ERROR_CREATE_NODES             = 2,
    ERROR_TREE_VERIFICATION        = 5,
    ERROR_NODE_DTOR                = 6,
    ERROR_TREE_INCORRECT_OPERATION = 8,
    ERROR_TREE_CHECK_ERROR         = 11,
    ERROR_SAVE_NODE                = 12,
    ERROR_TREE_INSERT              = 13,
    ERROR_READ_NODE                = 14,
    ERROR_SYNTAX                   = 17,
    ERROR_WRONG_TREE_INSERT_PATH   = 18,
    ERROR_READ_VALUE               = 19,
};

enum TreeInsertPath
{
    TREE_INSERT_FIRST = 0,
    TREE_INSERT_LEFT  = 1,
    TREE_INSERT_RIGHT = 2,
};

enum TreeErrorMask
{
    //for TreeVerification 
};

struct TreeNode
{
    tree_elem_t value = POISON_TREE_VALUE;
    struct TreeNode *left  = NULL;
    struct TreeNode *right = NULL;
    struct TreeNode *parent  = NULL;
};

struct Tree
{
    struct TreeNode *root = NULL;
    unsigned int size     = 0;

    int error = 0;
};

struct Tree *TreeCtor(void);
int TreeDtor(struct Tree *tree);
int TreeNodeDtor(struct TreeNode *curr_node);

int TreeInsert(struct Tree *tree, struct TreeNode *prev_node,
               int insert_path, tree_elem_t arg);
    
int TreeRemove(struct Tree *tree, struct TreeNode **node_ptr,
               tree_elem_t *arg);
    
int TreeVerification(struct Tree *tree);
int TreeCheckError(struct Tree *tree);

int TreeDump(const struct Tree *tree, const char *comment = NULL, bool debug = false,
             const char *file_name = NULL, int line_num = 0, const char *func_name = NULL);
int CreateTreeGraph(const struct Tree *tree);
int SaveTreeGraphPng(void);
int AddTreeGraphPng(int graph_counter);
int CreateTreeNodes(const struct TreeNode *curr_node, FILE *graph_f);

int TreeSerialize(const struct Tree *tree);
int SerializeNode(const struct TreeNode *curr_node, FILE *tree_f);

struct Tree *DeserializeTree(const char *input_file_name);
int DeserializeNode(struct Tree *new_tree, struct TreeNode *prev_node, char **buf);
int ReadValue(char *value, char **buf);


#endif //TREE_H_INCLUDED