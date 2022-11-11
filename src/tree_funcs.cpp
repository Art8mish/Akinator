
#include "../include/akinator.h"

#define VERIFICATION()                                                  \
            do                                                          \
            {                                                           \
                int verification_err = treeVerification(tree);          \
                ERROR_CHECK(verification_err, ERROR_TREE_VERIFICATION); \
                if (tree->error != 0)                                   \
                    return ERROR_TREE;                                  \
            } while(false)

#define INSPECT_TREE(cond, err_code)                \
            do                                      \
            {                                       \
                if (cond)                           \
                    tree->error |= err_code;        \
            } while(false) 

#define SKIP_SPACE(buf)                                     \
            while(isspace((int)(*buf)) || (*buf) == '\0')   \
            {                                               \
                buf++;                                      \
                if ((*buf) == EOF)                         \
                    return SUCCESS;                         \
            }   
                        

struct Tree *TreeCtor(void)
{
    struct Tree *tree = (struct Tree *) calloc(1, sizeof(struct Tree));

    tree->root = NULL;
    tree->size = 0;

    int lst_err = listCtor(&tree->lst_ret);
    ERROR_CHECK(lst_err, NULL);
    
    return tree;
}

int TreeDtor(struct Tree *tree)
{
    ERROR_CHECK(tree == NULL, ERROR_NULL_PTR);

    VERIFICATION();

    tree->size = 0;

    int lst_err = listDtor(&tree->lst_ret);
    ERROR_CHECK(lst_err, ERROR_TREE_DTOR);

    int node_dtor_err = treeNodeDtor(tree->root);
    ERROR_CHECK(node_dtor_err, ERROR_NODE_DTOR);

    tree->error |= CODE_DEAD_TREE;

    return SUCCESS;
}

int treeNodeDtor(struct TreeNode *curr_node)
{
    ERROR_CHECK(curr_node == NULL, ERROR_NULL_PTR);

    if (curr_node->left_yes != NULL)
    {
        int node_dtor_err = treeNodeDtor(curr_node->left_yes);
        ERROR_CHECK(node_dtor_err, ERROR_NODE_DTOR);
    }

    if (curr_node->rght_no != NULL)
    {
        int node_dtor_err = treeNodeDtor(curr_node->rght_no);
        ERROR_CHECK(node_dtor_err, ERROR_NODE_DTOR);
    }

    FREE_TREE_NODE_VALUE(curr_node);
    free(curr_node);

    return SUCCESS;
}

int treeInsert(struct Tree *tree, struct TreeNode **node_ptr, 
               tree_elem_t arg)
{
    ERROR_CHECK(tree     == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(node_ptr == NULL, ERROR_NULL_PTR);

    VERIFICATION();

    struct TreeNode *curr_node = (struct TreeNode *) calloc(1, sizeof(struct TreeNode));
    ERROR_CHECK(curr_node == NULL, ERROR_CALLOC);

    WRITE_TREE_NODE_VALUE(curr_node, arg);
    
    curr_node->rght_no = NULL;
    curr_node->left_yes  = NULL;

    *node_ptr = curr_node;

    tree->size += 1;

    VERIFICATION();    

    return SUCCESS;
}

int treeRemove(struct Tree *tree, struct TreeNode **node_ptr,
               tree_elem_t *arg)
{
    ERROR_CHECK(tree     == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(node_ptr == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(arg      == NULL, ERROR_NULL_PTR);

    VERIFICATION();

    ERROR_CHECK(tree->size == 0, ERROR_INCORRECT_OPERATION);

    if ((*node_ptr)->rght_no != NULL)
    {
        int remove_err = treeRemove(tree, &(*node_ptr)->rght_no, arg);
        ERROR_CHECK(remove_err, ERROR_TREE_REMOVE);
    }

    if ((*node_ptr)->left_yes != NULL)
    {
        int remove_err = treeRemove(tree, &(*node_ptr)->left_yes, arg);
        ERROR_CHECK(remove_err, ERROR_TREE_REMOVE);
    }

    *arg = (*node_ptr)->value;

    free(*node_ptr);
    *node_ptr = NULL;
    tree->size -= 1;

    VERIFICATION();
    
    return SUCCESS;
}

int treeVerification(struct Tree *tree)
{
    ERROR_CHECK(tree == NULL, ERROR_NULL_PTR);

    int tree_check_err = treeCheckError(tree);
    ERROR_CHECK(tree_check_err, ERROR_LIST_CHECK_ERROR);
    
    if (tree->error != 0)
        TREEDUMP(tree, "ERROR IN VERIFICATION");

    return SUCCESS;
}

int treeCheckError(struct Tree *tree)
{
    ERROR_CHECK(tree == NULL, ERROR_NULL_PTR);

    INSPECT_TREE(tree == NULL, ERROR_CODE_NULL_TREE);

    return SUCCESS;
}


int treeSave(const struct Tree *tree)
{
    ERROR_CHECK(tree == NULL, ERROR_NULL_PTR);

    FILE *tree_f = fopen(TREE_IO, "w");
    ERROR_CHECK(tree_f == NULL, ERROR_OPENING_FILE);

    if (tree->root != NULL)
    {
        int save_node_err = SaveNode(tree->root, tree_f);
        FILE_ERROR_CHECK(save_node_err, ERROR_SAVE_NODE, tree_f);
    }

    int fclose_err = fclose(tree_f);
    ERROR_CHECK(fclose_err != 0, ERROR_CLOSING_FILE);

    return SUCCESS;
}

int SaveNode(const struct TreeNode *curr_node, FILE *tree_f)
{
    ERROR_CHECK(curr_node == NULL, ERROR_NULL_PTR);

    bool have_children = false;
    static unsigned int recur_count = 0;
    recur_count++;

    fprintf(tree_f, "\n");
    for (unsigned int i = 1; i < recur_count; i++)
        fprintf(tree_f, "\t");

    if (curr_node != NULL)
        fprintf(tree_f, "{ \"%s\" ", curr_node->value);

    if (curr_node->left_yes != NULL)
    {
        have_children = true;
        int save_node_err = SaveNode(curr_node->left_yes, tree_f);
        ERROR_CHECK(save_node_err, ERROR_SAVE_NODE);
    }

    if (curr_node->rght_no != NULL)
    {
        have_children = true;
        int save_node_err = SaveNode(curr_node->rght_no, tree_f);
        ERROR_CHECK(save_node_err, ERROR_SAVE_NODE);
    }

    if (have_children)
        for (unsigned int i = 1; i < recur_count; i++)
            fprintf(tree_f, "\t");

    fprintf(tree_f, "}\n");

    recur_count--;

    return SUCCESS;
}

struct Tree *ReadTree(const char *input_file_name)
{
    ERROR_CHECK(input_file_name == NULL, NULL);

    struct Tree *new_tree = TreeCtor();
    ERROR_CHECK(new_tree == NULL, NULL);

    struct WorkingField *onegin_context = CreateWorkingField(input_file_name);
    ERROR_CHECK(onegin_context == NULL, NULL);

    char *buf = onegin_context->chars_buffer;

    /*for (int i = 0; i < onegin_context->lines_amount; i++)
    {
        printf("%s\n", buf);
        buf += strlen(buf) + 1;
    }*/

    int read_node_err = ReadNode(new_tree, &new_tree->root, &buf);
    ERROR_CHECK(read_node_err, NULL);

    return new_tree;    
}

int ReadNode(struct Tree *new_tree, struct TreeNode **curr_node, char **buf)
{
    ERROR_CHECK(new_tree == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(curr_node == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(buf == NULL, ERROR_NULL_PTR);

    char value[MAX_STR_SIZE] = { 0 };
    char symb = 0;
    int symb_count = 0;

    SKIP_SPACE((*buf));
    printf("symb1 = %c\n", (**buf));
    printf("%s\n", (*buf));

    if ((**buf) != '{')
        return ERROR_SYNTAX;

    (*buf)++;
    sscanf((*buf), " \"" READABLE_SYMB "\"%n", value,  &symb_count);
    printf("value = %s, symb_count = %d\n", value, symb_count);
    (*buf) += symb_count;

    int insert_err = treeInsert(new_tree, curr_node, value);
    TREEDUMP(new_tree, "отлажу хуила");
    ERROR_CHECK(insert_err, ERROR_TREE_INSERT);

    SKIP_SPACE((*buf));
    printf("symb = %c\n", (**buf));
    printf("%s\n", (*buf));


    if ((**buf) == '}')
    {
        (*buf)++;
        return SUCCESS;
    }

    if ((**buf) == '{')
    {
        printf("zashel v leviy\n");
        printf("%s\n", (*buf));
        int read_node_err = ReadNode(new_tree, &(*curr_node)->left_yes, buf);
        ERROR_CHECK(read_node_err, ERROR_READ_NODE);

        SKIP_SPACE((*buf));
        printf("symb = %c\n", symb);


        printf("zashel v praviy\n");
        printf("%s\n", (*buf));
        read_node_err = ReadNode(new_tree, &(*curr_node)->rght_no, buf);
        ERROR_CHECK(read_node_err, ERROR_READ_NODE);
    }

    TREEDUMP(new_tree, "сука");

    return SUCCESS;
}

