
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


int Greeting(int *mode)
{
    ERROR_CHECK(mode == NULL, ERROR_NULL_PTR);

    printf("This is akinator, I can guess your word.\n"
            "Choose mode:\n"
            "Enter 1 for Guessing;\n"
            "Enter 2 for Giving definition;\n"
            "Enter 3 for Comparising two objects;\n"
            "Enter 4 for Creating  Treegraph;\n\n"
            "Enter 5 to Exit with saving concepts;\n"
            "Enter 6 to Exit without saving concepts;\n");

    CHECK_INSERT(scanf("%d", mode), *mode >= 1 && *mode <= 4);

    return SUCCESS;
}

int GuessConcept(struct Tree *tree)
{
    ERROR_CHECK(tree == NULL, ERROR_NULL_PTR);

    if (tree->root == NULL)
        return ERROR_NULL_TREE_ROOT;

    int guess_iter_err = DoGuessIter(tree, tree->root);
    ERROR_CHECK(guess_iter_err, ERROR_GUESS_ITER);

    //int akinator_err = StartAkinator();
    //ERROR_CHECK(akinator_err, ERROR_AKINATOR);

    return SUCCESS;
}

int DoGuessIter(struct Tree *tree, struct TreeNode *curr_node)
{
    ERROR_CHECK(curr_node == NULL, ERROR_NULL_PTR);

    printf(TREE_SPECIFIER "\n", curr_node->value);

    char answer[4] = {};
    CHECK_INSERT(scanf(" %3s", answer), strcmp(answer, "yes") == 0 || 
                                        strcmp(answer, "no")  == 0);

    if (strcmp(answer, "no") == 0)
    {
        if (curr_node->rght_no == NULL)
        {
            int add_new_concept = AddNewConcept(tree, curr_node);
            ERROR_CHECK(add_new_concept, ERROR_ADD_NEW_CONCEPT);
        }
        else
        {
            int guess_iter_err = DoGuessIter(tree, curr_node->rght_no);
            ERROR_CHECK(guess_iter_err, ERROR_GUESS_ITER);
        }
    }

    else if (strcmp(answer, "yes") == 0)
    {
        if (curr_node->left_yes == NULL)
        {
            printf("I guessed right again. I'm very usefull!");

            return SUCCESS;
        }

        else
        {
            int guess_iter_err = DoGuessIter(tree, curr_node->left_yes);
            ERROR_CHECK(guess_iter_err, ERROR_GUESS_ITER);
        }
    }

    else    
        return ERROR_INCORRECT_ANSWER;

    return SUCCESS;
}

int AddNewConcept(struct Tree *tree, struct TreeNode *curr_node)
{
    ERROR_CHECK(curr_node == NULL, ERROR_NULL_PTR);

    printf("You won! I don't know what it is.\n"
           "Please, tell me what you wished:\n");

    char user_word[MAX_STR_SIZE] = { 0 };
    CHECK_INSERT(scanf(" " READABLE_SYMB, user_word), strlen(user_word) < MAX_STR_SIZE);

    printf("Interesting, let's add this concept.\n"
           "Please, tell me without using \"no\" how does %s differ from %s (100 symbols):\n", user_word, curr_node->value);
    char difference[MAX_STR_SIZE] = { 0 };
    CHECK_INSERT(scanf(" " READABLE_SYMB, difference), strlen(difference) < MAX_STR_SIZE);

    int insert_err = treeInsert(tree, &curr_node->rght_no, curr_node->value);
    ERROR_CHECK(insert_err, ERROR_TREE_INSERT);

        insert_err = treeInsert(tree, &curr_node->left_yes, user_word);
    ERROR_CHECK(insert_err, ERROR_TREE_INSERT);

    WRITE_TREE_NODE_VALUE(curr_node, difference);

    int save_node_err = treeSave(tree);
    ERROR_CHECK(save_node_err, ERROR_TREE_SAVE);

    return SUCCESS;
}