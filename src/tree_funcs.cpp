
#include "../include/tree.h"

#define VERIFICATION(tree)                                              \
            do                                                          \
            {                                                           \
                int verification_err = TreeVerification(tree);          \
                ERROR_CHECK(verification_err, ERROR_TREE_VERIFICATION); \
                if (tree->error != 0)                                   \
                    return ERROR_TREE;                                  \
            } while(false)

#define INSPECT_TREE(cond, err_code)                \
            do                                      \
            {                                       \
                if (cond)                           \
                    tree->error |= (err_code);      \
            } while(false) 

#define SKIP_SPACE(buf)                                     \
            while(isspace((int)(*buf)) || (*buf) == '\0')   \
            {                                               \
                (buf)++;                                    \
                if ((*buf) == EOF)                          \
                    return SUCCESS;                         \
            }   
                        

struct Tree *TreeCtor(void)
{
    struct Tree *tree = (struct Tree *) calloc(1, sizeof(struct Tree));
    ERROR_CHECK(tree == NULL, NULL);

    tree->root = NULL;
    tree->size = 0;
    
    return tree;
}

int TreeDtor(struct Tree *tree)
{
    ERROR_CHECK(tree == NULL, ERROR_NULL_PTR);

    VERIFICATION(tree);

    tree->size = 0;

    int node_dtor_err = TreeNodeDtor(tree->root);
    ERROR_CHECK(node_dtor_err, ERROR_NODE_DTOR);

    free(tree);

    return SUCCESS;
}

int TreeNodeDtor(struct TreeNode *curr_node)
{
    ERROR_CHECK(curr_node == NULL, ERROR_NULL_PTR);

    if (curr_node->left != NULL)
    {
        int node_dtor_err = TreeNodeDtor(curr_node->left);
        ERROR_CHECK(node_dtor_err, ERROR_NODE_DTOR);
    }

    if (curr_node->right != NULL)
    {
        int node_dtor_err = TreeNodeDtor(curr_node->right);
        ERROR_CHECK(node_dtor_err, ERROR_NODE_DTOR);
    }

    curr_node->parent = NULL;
    FREE_TREE_NODE_VALUE(curr_node);
    free(curr_node);

    return SUCCESS;
}

int TreeInsert(struct Tree *tree, struct TreeNode *prev_node,
               int insert_path, tree_elem_t arg)
{
    ERROR_CHECK(tree == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(insert_path != TREE_INSERT_LEFT  &&
                insert_path != TREE_INSERT_RIGHT &&
                insert_path != TREE_INSERT_FIRST, ERROR_WRONG_TREE_INSERT_PATH);

    VERIFICATION(tree);

    struct TreeNode *curr_node = (struct TreeNode *) calloc(1, sizeof(struct TreeNode));
    ERROR_CHECK(curr_node == NULL, ERROR_CALLOC);

    WRITE_TREE_NODE_VALUE(curr_node, arg);
    
    curr_node->right = NULL;
    curr_node->left  = NULL;
    curr_node->parent  = prev_node;

    switch (insert_path)
    {
        case TREE_INSERT_FIRST : tree->root = curr_node;
                                 break;

        case TREE_INSERT_LEFT  : ERROR_CHECK(prev_node == NULL, ERROR_NULL_PTR);
                                 prev_node->left  = curr_node;
                                 break;

        case TREE_INSERT_RIGHT : ERROR_CHECK(prev_node == NULL, ERROR_NULL_PTR);
                                 prev_node->right = curr_node;
                                 break;
        
        default                : return ERROR_WRONG_TREE_INSERT_PATH;
                                 break;
    }

    tree->size += 1;

    VERIFICATION(tree);    

    return SUCCESS;
}

int TreeRemove(struct Tree *tree, struct TreeNode **node_ptr,
               tree_elem_t *arg)
{
    ERROR_CHECK(tree     == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(node_ptr == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(arg      == NULL, ERROR_NULL_PTR);

    VERIFICATION(tree);

    ERROR_CHECK(tree->size == 0, ERROR_TREE_INCORRECT_OPERATION);

    *arg = (*node_ptr)->value;

    int node_dtor_err = TreeNodeDtor(*node_ptr);
    ERROR_CHECK(node_dtor_err, ERROR_NODE_DTOR);

    *node_ptr = NULL;
    tree->size -= 1;

    VERIFICATION(tree);
    
    return SUCCESS;
}

int TreeVerification(struct Tree *tree)
{
    ERROR_CHECK(tree == NULL, ERROR_NULL_PTR);

    int tree_check_err = TreeCheckError(tree);
    ERROR_CHECK(tree_check_err, ERROR_TREE_CHECK_ERROR);
    
    if (tree->error != 0)
        TREEDUMP(tree, "ERROR IN VERIFICATION");

    return SUCCESS;
}


int TreeCheckError(struct Tree *tree)
{
    ERROR_CHECK(tree == NULL, ERROR_NULL_PTR);

    //INCPECT_TREE(CONDITION, ERROR_CODE)

    return SUCCESS;
}

int TreeSerialize(const struct Tree *tree)
{
    ERROR_CHECK(tree == NULL, ERROR_NULL_PTR);

    FILE *tree_f = fopen(TREE_SERIALIZATION_PATH, "w");
    ERROR_CHECK(tree_f == NULL, ERROR_OPENING_FILE);

    if (tree->root != NULL)
    {
        int save_node_err = SerializeNode(tree->root, tree_f);
        FILE_ERROR_CHECK(save_node_err, ERROR_SAVE_NODE, tree_f);
    }

    int fclose_err = fclose(tree_f);
    ERROR_CHECK(fclose_err != 0, ERROR_CLOSING_FILE);

    return SUCCESS;
}

int SerializeNode(const struct TreeNode *curr_node, FILE *tree_f)
{
    ERROR_CHECK(curr_node == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(tree_f    == NULL, ERROR_NULL_PTR);

    bool child_ex = false;
    static unsigned int recur_count = 0;
    recur_count++;

    fprintf(tree_f, "\n");
    for (unsigned int i = 1; i < recur_count; i++)
        fprintf(tree_f, "\t");

    if (curr_node != NULL)
        fprintf(tree_f, "{ \"" TREE_SPECIFIER "\" ", curr_node->value);

    if (curr_node->left != NULL)
    {
        child_ex = true;
        int save_node_err = SerializeNode(curr_node->left, tree_f);
        ERROR_CHECK(save_node_err, ERROR_SAVE_NODE);
    }

    if (curr_node->right != NULL)
    {
        child_ex = true;
        int save_node_err = SerializeNode(curr_node->right, tree_f);
        ERROR_CHECK(save_node_err, ERROR_SAVE_NODE);
    }

    if (child_ex)
        for (unsigned int i = 1; i < recur_count; i++)
            fprintf(tree_f, "\t");

    fprintf(tree_f, "}\n");

    recur_count--;

    return SUCCESS;
}

struct Tree *DeserializeTree(const char *input_file_name)
{
    ERROR_CHECK(input_file_name == NULL, NULL);

    struct Tree *new_tree = TreeCtor();
    ERROR_CHECK(new_tree == NULL, NULL);

    struct WorkingField *onegin_context = CreateWorkingField(input_file_name);
    ERROR_CHECK(onegin_context == NULL, NULL);

    char *buf = onegin_context->chars_buffer;

    char value[MAX_TREE_STR_SIZE] = { 0 };
    int read_val_err = ReadValue(value, &buf);
    ERROR_CHECK(read_val_err, NULL);

    int insert_err = TreeInsert(new_tree, NULL, TREE_INSERT_FIRST, value);
    ERROR_CHECK(insert_err, NULL);

    int read_node_err = DeserializeNode(new_tree, new_tree->root, &buf);
    ERROR_CHECK(read_node_err, NULL);

    return new_tree;    
}

int DeserializeNode(struct Tree *new_tree, struct TreeNode *prev_node, char **buf)
{
    ERROR_CHECK(new_tree  == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(prev_node == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(buf       == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(*buf      == NULL, ERROR_NULL_PTR);

    SKIP_SPACE((*buf));

    if ((**buf) == '}')
    {
        (*buf)++;
        return SUCCESS;
    }

    if ((**buf) == '{')
    {
        char value[MAX_TREE_STR_SIZE] = { 0 };
        int read_val_err = ReadValue(value, buf);
        ERROR_CHECK(read_val_err, ERROR_READ_VALUE);

        int insert_err = TreeInsert(new_tree, prev_node, TREE_INSERT_LEFT, value);
        ERROR_CHECK(insert_err, ERROR_TREE_INSERT);

        int read_node_err = DeserializeNode(new_tree, prev_node->left, buf);
        ERROR_CHECK(read_node_err, ERROR_READ_NODE);

        SKIP_SPACE((*buf));

            read_val_err = ReadValue(value, buf);
        ERROR_CHECK(read_val_err, ERROR_READ_VALUE);

            insert_err = TreeInsert(new_tree, prev_node, TREE_INSERT_RIGHT, value);
        ERROR_CHECK(insert_err, ERROR_TREE_INSERT);

            read_node_err = DeserializeNode(new_tree, prev_node->right, buf);
        ERROR_CHECK(read_node_err, ERROR_READ_NODE);
    }

    SKIP_SPACE((*buf))

    if (**buf != '}')
        return ERROR_SYNTAX;

    (*buf)++;

    return SUCCESS;
}

int ReadValue(char *value, char **buf)
{
    ERROR_CHECK( buf  == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(*buf  == NULL, ERROR_NULL_PTR);

    int symb_count = 0;

    SKIP_SPACE((*buf));
    (*buf)++;
    sscanf((*buf), " \"" READABLE_SYMB "\"%n", value,  &symb_count);
    (*buf) += symb_count;

    return SUCCESS;
}

