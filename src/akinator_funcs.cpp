
#include "../include/akinator.h"

#define CHECK_INSERT(scanf_code, condition)                                      \
        do                                                                       \
        {                                                                        \
            bool correct_input = false;                                          \
            while (!correct_input)                                               \
            {                                                                    \
                int scanf_ret = scanf_code;                                      \
                if (scanf_ret && (condition))                                    \
                    correct_input = true;                                        \
                int ch = 0;                                                      \
                while ((ch = getchar()) != '\n')                                 \
                    if (!isspace(ch))                                            \
                    {                                                            \
                        correct_input = false;                                   \
                        break;                                                   \
                    }                                                            \
                if (!correct_input)                                              \
                    printf("I do not understand.\nEnter correct value..\n");     \
            }                                                                    \
        } while(false)

#define CTOR_LIST_RET(list_ret, desired_node)                                             \
            struct List *(list_ret) = (struct List *) calloc(1, sizeof(struct List));       \
            do                                                                              \
            {                                                                               \
                int list_ctor_err = listCtor(list_ret);                                     \
                ERROR_CHECK(list_ctor_err, ERROR_CTOR);                                     \
                int full_ret_list_err = FillRetList((list_ret), (desired_node));            \
                ERROR_CHECK(full_ret_list_err, ERROR_FULL_RET_LIST);                        \
            } while (false)

#define DTOR_LIST_RET(list_ret)                                     \
            do                                                      \
            {                                                       \
                int list_dtor_err = listDtor(list_ret);             \
                ERROR_CHECK(list_dtor_err, ERROR_DTOR);             \
                free(list_ret);                                     \
            } while (false)
            

struct Akinator *AkinatorCtor(void)
{
    struct Akinator *akn = (struct Akinator *) calloc(1, sizeof(struct Akinator));

    akn->tree = DeserializeTree(TREE_SERIALIZATION_PATH);
    ERROR_CHECK(akn->tree == NULL, NULL);

    TREEDUMP(akn->tree, "ctor dump");

    return akn;    
}

int AkinatorDtor(struct Akinator *akn)
{
    ERROR_CHECK(akn == NULL, ERROR_NULL_PTR);

    int tree_dtor_err = TreeDtor(akn->tree);
    ERROR_CHECK(tree_dtor_err, ERROR_TREE_DTOR);
    akn->tree = NULL;

    free(akn);

    return SUCCESS;
}

int Greeting(int *mode)
{
    ERROR_CHECK(mode == NULL, ERROR_NULL_PTR);

    printf("This is akinator, I can guess your word.\n"
            "Choose mode:\n"
            "Enter 1 for Guessing;\n"
            "Enter 2 for Giving definition;\n"
            "Enter 3 for Comparison two objects;\n"
            "Enter 4 for Creating TreeGraph;\n\n"
            "Enter 5 to Exit with saving concepts;\n"
            "Enter 6 to Exit without saving concepts;\n");

    CHECK_INSERT(scanf("%d", mode), *mode >= 1 && *mode <= 6);

    return SUCCESS;
}

int GuessConcept(struct Akinator *akn)
{
    ERROR_CHECK(akn       == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(akn->tree == NULL, ERROR_NULL_PTR);

    if (akn->tree->root == NULL)
        return ERROR_NULL_TREE_ROOT;

    int guess_iter_err = DoGuessIter(akn, akn->tree->root);
    ERROR_CHECK(guess_iter_err, ERROR_GUESS_ITER);

    return SUCCESS;
}

int DoGuessIter(struct Akinator *akn, struct TreeNode *curr_node)
{
    ERROR_CHECK(akn       == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(akn->tree == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(curr_node == NULL, ERROR_NULL_PTR);

    if (curr_node->left == NULL && curr_node->right == NULL)
    {
        int make_guess = MakeGuess(akn, curr_node);
        ERROR_CHECK(make_guess, ERROR_MAKE_GUESS);

        return SUCCESS;    
    }

    char answer[MAX_CMD_SIZE] = { 0 };
    printf(TREE_SPECIFIER "?\n", curr_node->value);
    CHECK_INSERT(scanf(" %3s", answer), strcmp(answer, "yes") == 0 || 
                                        strcmp(answer, "no")  == 0);

    if (strcmp(answer, "no") == 0)
    {
        int guess_iter_err = DoGuessIter(akn, curr_node->right);
        ERROR_CHECK(guess_iter_err, ERROR_GUESS_ITER);
    }

    else if (strcmp(answer, "yes") == 0)
    {
        int guess_iter_err = DoGuessIter(akn, curr_node->left);
        ERROR_CHECK(guess_iter_err, ERROR_GUESS_ITER);
    }

    else    
        return ERROR_INCORRECT_ANSWER;

    return SUCCESS;
}

int MakeGuess(struct Akinator *akn, struct TreeNode *curr_node)
{
    ERROR_CHECK(akn       == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(akn->tree == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(curr_node == NULL, ERROR_NULL_PTR);

    char answer[MAX_CMD_SIZE] = { 0 };

    printf("I think, this is " TREE_SPECIFIER "\n", curr_node->value);
    printf("Am I right?\n");
    CHECK_INSERT(scanf(" %3s", answer), strcmp(answer, "yes") == 0 || 
                                        strcmp(answer, "no")  == 0);
    if (strcmp(answer, "yes") == 0)
    {
        printf("I guessed right. I'm very usefull!\n\n");

        return SUCCESS;
    }

    else if (strcmp(answer, "no") == 0)
    {
        printf("You won! I don't know what it is.\n\n");
        printf("Do you want to give definition? (yes/no)\n");
        CHECK_INSERT(scanf(" %3s", answer), strcmp(answer, "yes") == 0 || 
                                            strcmp(answer, "no")  == 0);
        if (strcmp(answer, "yes") == 0)
        {
            int add_new_concept = AddNewConcept(akn, curr_node);
            ERROR_CHECK(add_new_concept, ERROR_ADD_NEW_CONCEPT);
        }
    }

    return SUCCESS;
}

int AddNewConcept(struct Akinator *akn, struct TreeNode *curr_node)
{
    ERROR_CHECK(akn       == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(akn->tree == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(curr_node == NULL, ERROR_NULL_PTR);

    printf("Please, tell me what you wished:\n");

    char user_word[MAX_STR_SIZE] = { 0 };
    CHECK_INSERT(scanf(" " READABLE_SYMB, user_word), strlen(user_word) < MAX_STR_SIZE);

    printf("Interesting, let's add this concept.\n"
           "Please, tell me without using \"not\" how does %s differ from %s (100 symbols):\n", user_word, curr_node->value);
    char difference[MAX_STR_SIZE] = { 0 };
    CHECK_INSERT(scanf(" " READABLE_SYMB, difference), strlen(difference) < MAX_STR_SIZE);

    int insert_err = TreeInsert(akn->tree, curr_node, TREE_INSERT_RIGHT, curr_node->value);
    ERROR_CHECK(insert_err, ERROR_TREE_INSERT);

        insert_err = TreeInsert(akn->tree, curr_node, TREE_INSERT_LEFT, user_word);
    ERROR_CHECK(insert_err, ERROR_TREE_INSERT);

    WRITE_TREE_NODE_VALUE(curr_node, difference);

    TREEDUMP(akn->tree, "Adding dump");

    return SUCCESS;
}

int MakeDefinition(struct Akinator *akn)
{
    ERROR_CHECK(akn       == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(akn->tree == NULL, ERROR_NULL_PTR);

    char answer[MAX_STR_SIZE] = { 0 };
    printf("What do you want to define?\n");

    CHECK_INSERT(scanf(" " READABLE_SYMB, answer), (strlen(answer) < MAX_STR_SIZE));

    struct TreeNode *desired_node = NULL;
    int definition_iter_err = FindDesiredNode(akn, akn->tree->root, &desired_node, answer);
    ERROR_CHECK(definition_iter_err, ERROR_DEFINITION_ITER_ERROR);

    if (desired_node == NULL)
    {
        printf("I don't know this concept...\n\n");

        return SUCCESS;
    }

    printf(TREE_SPECIFIER ": ", desired_node->value);

    int print_definition_err = PrintDefinition(desired_node);
    ERROR_CHECK(print_definition_err, ERROR_PRINT_DEFINITION);
    printf("\n\n");

    return SUCCESS;
}

int FindDesiredNode(struct Akinator *akn, struct TreeNode *curr_node,
                    struct TreeNode **desired_node, tree_elem_t concept)
{
    ERROR_CHECK(akn          == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(akn->tree    == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(curr_node    == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(desired_node == NULL, ERROR_NULL_PTR);

    if (COMPARE_TREE_ELEM(curr_node->value, concept))
    {
        *desired_node = curr_node;
        return SUCCESS;
    }

    if (curr_node->left != NULL)
    {
        int definition_iter_err = FindDesiredNode(akn, curr_node->left, desired_node, concept);
        ERROR_CHECK(definition_iter_err, ERROR_DEFINITION_ITER);
    }

    if (curr_node->right != NULL)
    {
        int definition_iter_err = FindDesiredNode(akn, curr_node->right, desired_node, concept);
        ERROR_CHECK(definition_iter_err, ERROR_DEFINITION_ITER);
    }
    
    return SUCCESS;
}

int PrintDefinition(struct TreeNode *desired_node)
{
    ERROR_CHECK(desired_node == NULL, ERROR_NULL_PTR);

    CTOR_LIST_RET(list_ret, desired_node);

    int print_list_ret = PrintListRet(list_ret, desired_node);
    ERROR_CHECK(print_list_ret, ERROR_PRINT_LIST_RET);

    DTOR_LIST_RET(list_ret);

    return SUCCESS;
}

int FillRetList(struct List *list_ret, struct TreeNode *curr_node)
{
    ERROR_CHECK(list_ret  == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(curr_node == NULL, ERROR_NULL_PTR);

    struct Node *list_node = NULL;
    int list_push_err = listPushBack(list_ret, curr_node, &list_node);
    ERROR_CHECK(list_push_err, ERROR_LIST_INSERT_AFTER);

    if (curr_node->prev == NULL)
        return SUCCESS;

    int print_definition_err = FillRetList(list_ret, curr_node->prev);
    ERROR_CHECK(print_definition_err, ERROR_PRINT_DEFINITION);

    return SUCCESS;
}

int CompareConcepts(struct Akinator *akn)
{
    ERROR_CHECK(akn == NULL, ERROR_NULL_PTR);

    char frst_cncpt[MAX_STR_SIZE] = { 0 };
    char scnd_cncpt[MAX_STR_SIZE] = { 0 };
    printf("What do you want to compare?\n");

    printf("Insert first concept: ");
    CHECK_INSERT(scanf(" " READABLE_SYMB, frst_cncpt), (strlen(frst_cncpt) < MAX_STR_SIZE));

    printf("Insert second concept: ");
    CHECK_INSERT(scanf(" " READABLE_SYMB, scnd_cncpt), (strlen(scnd_cncpt) < MAX_STR_SIZE));

    struct TreeNode *frst_cncpt_node  = NULL;
    int definition_iter_err = FindDesiredNode(akn, akn->tree->root, &frst_cncpt_node, frst_cncpt);
    ERROR_CHECK(definition_iter_err, ERROR_DEFINITION_ITER_ERROR);

    if (strcmp(frst_cncpt, scnd_cncpt) == 0)
    {
        printf("You write same concepts:\n");
        printf(TREE_SPECIFIER ": ", frst_cncpt);

        int print_definition_err = PrintDefinition(frst_cncpt_node);
        ERROR_CHECK(print_definition_err, ERROR_PRINT_DEFINITION);
        printf("\n\n");

        return SUCCESS;
    }

    struct TreeNode *scnd_cncpt_node = NULL;
        definition_iter_err = FindDesiredNode(akn, akn->tree->root, &scnd_cncpt_node, scnd_cncpt);
    ERROR_CHECK(definition_iter_err, ERROR_DEFINITION_ITER_ERROR);

    int print_comparison_err = PrintComparison(akn, frst_cncpt_node, scnd_cncpt_node);
    ERROR_CHECK(print_comparison_err, ERROR_PRINT_COMPARISON);

    return SUCCESS;
}

int PrintComparison(struct Akinator *akn, struct TreeNode *frst_node,
                                          struct TreeNode *scnd_node)
{
    ERROR_CHECK(akn       == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(akn->tree == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(frst_node == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(scnd_node == NULL, ERROR_NULL_PTR);
  
    CTOR_LIST_RET(frst_list_ret, frst_node);
    CTOR_LIST_RET(scnd_list_ret, scnd_node);

    struct TreeNode *curr_node = NULL;
    do
    {
        int list_pop_err = listPopBack(frst_list_ret, &curr_node);
        ERROR_CHECK(list_pop_err, ERROR_POP_BACK);

            list_pop_err = listPopBack(scnd_list_ret, &curr_node);
        ERROR_CHECK(list_pop_err, ERROR_POP_BACK);

    } while(frst_list_ret->fict_node->prev->value == 
            scnd_list_ret->fict_node->prev->value);

    struct TreeNode *dividing_node = curr_node;
    printf("%s\n", curr_node->value);
    if (dividing_node != akn->tree->root)
    {
        printf("They both: ");
        int print_def_err = PrintDefinition(dividing_node);
        ERROR_CHECK(print_def_err, ERROR_PRINT_DEFINITION);
        printf("but, ");
    }

    else
        printf("They have nothing common\n");

    
    struct Node *list_node = NULL;
    int list_push_err = listPushBack(frst_list_ret, dividing_node, &list_node);
    ERROR_CHECK(list_push_err, ERROR_LIST_INSERT_AFTER);

    list_push_err = listPushBack(scnd_list_ret, dividing_node, &list_node);
    ERROR_CHECK(list_push_err, ERROR_LIST_INSERT_AFTER);
    

    printf("\n" TREE_SPECIFIER ": ", frst_node->value);
    int print_list_ret_err = PrintListRet(frst_list_ret, frst_node);
    ERROR_CHECK(print_list_ret_err, ERROR_PRINT_LIST_RET);

    printf("\nand " TREE_SPECIFIER ": ", scnd_node->value);
        print_list_ret_err = PrintListRet(scnd_list_ret, scnd_node);
    ERROR_CHECK(print_list_ret_err, ERROR_PRINT_LIST_RET);
    printf("\n\n");

    DTOR_LIST_RET(frst_list_ret);
    DTOR_LIST_RET(scnd_list_ret);

    return SUCCESS;
}

int PrintListRet(struct List *list_ret, struct TreeNode *desired_node)
{
    ERROR_CHECK(list_ret     == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(desired_node == NULL, ERROR_NULL_PTR);

    while (list_ret->fict_node->prev->value != desired_node)
    {
        int counter = 0;
        struct TreeNode *curr_node = NULL;
        int list_pop_err = listPopBack(list_ret, &curr_node);
        ERROR_CHECK(list_pop_err, ERROR_POP_BACK);

        if (list_ret->fict_node->prev->value == curr_node->right)
                printf("not ");
            
        printf(TREE_SPECIFIER, curr_node->value);

        if (list_ret->fict_node->prev->value != desired_node)
            printf(", ");

        counter++;
        if (counter == 20)
            return ERROR_AKINATOR;
    } 

    return SUCCESS;
}

int RunAkinator(struct Akinator *akn)
{
    ERROR_CHECK(akn       == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(akn->tree == NULL, ERROR_NULL_PTR);

    do
    {
        int mode = 0;
        int greet_err = Greeting(&mode);
        ERROR_CHECK(greet_err, ERROR_GREETING);

        switch (mode)
        {
            case MODE_GUESS : 
                    {
                        int guess_err = GuessConcept(akn);
                        ERROR_CHECK(guess_err, ERROR_GUESS_CONCEPT);
                        break;
                    }
                    
            case MODE_DEFINE : 
                    {
                        int make_def_err = MakeDefinition(akn);
                        ERROR_CHECK(make_def_err, ERROR_MAKE_DEFINITION);
                        break;
                    }

            case MODE_COMPARISON :
                    {
                        int compare_concepts_err = CompareConcepts(akn);
                        ERROR_CHECK(compare_concepts_err, ERROR_COMPARE_CONCEPTS);
                    }

            case MODE_DUMP :
                    {
                        TreeDump(akn->tree);
                        break;
                    }

            case MODE_SAVE_EXIT : 
                    {
                        int save_tree_err = TreeSerialize(akn->tree);
                        ERROR_CHECK(save_tree_err, ERROR_TREE_SAVE);
                        //falling further
                    }

            case MODE_EXIT : 
                    {
                        int akn_dtor_err = AkinatorDtor(akn);
                        ERROR_CHECK(akn_dtor_err, ERROR_AKINATOR_DTOR);
                        return SUCCESS;
                        break;
                    }
            
            default   : return ERROR_AKINATOR;
                        break;
        }
    } while(true);

    return SUCCESS;
}