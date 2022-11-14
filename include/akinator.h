#ifndef AKINATOR_H_INCLUDED
#define AKINATOR_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <locale.h>
#include <ctype.h>

#include "../list_functions/include/ptr_list.h"
#define PTR_LIST_H_INCLUDED

#include "tree.h"

#include "common.h"

#define COLOR_TEXT(text) "\e[31m" #text "\e[0m"

const size_t MAX_STR_SIZE = 101;
const size_t MAX_CMD_SIZE = 4;

#ifndef READABLE_SYMB
#define READABLE_SYMB "%[a-zA-z -]"
#endif

#include "error.h"

enum Mode
{
    MODE_GUESS      = 1,
    MODE_DEFINE     = 2,
    MODE_COMPARISON = 3,
    MODE_DUMP       = 4,
    MODE_SAVE_EXIT  = 5,
    MODE_EXIT       = 6,
};

struct Akinator
{
    struct Tree *tree     = NULL;
    struct List *list_ret = NULL;
};

int StartAkinator(struct Akinator *akn);

struct Akinator *AkinatorCtor(void);
int AkinatorDtor(struct Akinator *akn);

int Greeting(int *mode);

int GuessConcept(struct Akinator *akn);
int DoGuessIter(struct Akinator *akn, struct TreeNode *curr_node);
int MakeGuess(struct Akinator *akn, struct TreeNode *curr_node);

int AddNewConcept(struct Akinator *akn, struct TreeNode *curr_node);

int MakeDefinition(struct Akinator *akn);
int DoDefinitionIter(struct Akinator *akn, struct TreeNode *curr_node,
                     struct TreeNode **desired_node, tree_elem_t concept);
int PrintDefinition(struct Akinator *akn, struct TreeNode *desire_node);
int FillRetList(struct Akinator *akn, struct TreeNode *curr_node);

#endif //AKINATOR_H_INCLUDED