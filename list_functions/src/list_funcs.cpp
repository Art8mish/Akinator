
#include "../include/ptr_list.h"

#define VERIFICATION()                                                  \
            do                                                          \
            {                                                           \
                int verification_err = listVerification(list);          \
                ERROR_CHECK(verification_err, ERROR_VERIFICATION);      \
            } while(false)

#define FREE_NODE list->free_node
#define TAIL_NODE list->fict_node->prev
#define HEAD_NODE list->fict_node->next
#define CURR_NODE curr_node
#define FICT_NODE list->fict_node

#define INSPECT_LIST(cond, err_code)                \
            do                                      \
            {                                       \
                if (cond)                           \
                    list->error |= err_code;        \
            } while(false)  


int listCtor(struct List *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    list->fict_node = (struct Node *) calloc(1, sizeof(struct Node));
    ERROR_CHECK(list->fict_node == NULL, ERROR_CALLOC);

    list->fict_node->value = 0;
    list->fict_node->next = list->fict_node;
    list->fict_node->prev = list->fict_node;

    list->size  = 0;
    list->error = 0;

    int verification_err = listVerification(list);         
    ERROR_CHECK(verification_err, ERROR_VERIFICATION);

    return SUCCESS;
}

int listDtor(struct List *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    int verify_err = listVerification(list);
    ERROR_CHECK(verify_err, ERROR_VERIFICATION);

    ERROR_CHECK(list->fict_node == NULL, ERROR_NULL_PTR);
    struct Node *curr_node = list->fict_node;

    for (unsigned int i = 0; i < list->size; i++)
    {
        curr_node = curr_node->next;
        free(curr_node);
    }

    free(list->fict_node);
    list->fict_node = NULL;

    list->size = 0;

    list->error |= CODE_DEAD_LIST;

    return SUCCESS;
}


int listPushBack(struct List *list, list_elem_t arg,
                 struct Node **new_node_ptr)
{
    ERROR_CHECK(list         == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(new_node_ptr == NULL, ERROR_NULL_PTR);

    int list_insert_err = listInsertAfter(list, FICT_NODE->prev, arg, new_node_ptr);
    ERROR_CHECK(list_insert_err, ERROR_LIST_INSERT_AFTER);

    return SUCCESS;
}

int listPushFront(struct List *list, list_elem_t arg,
                  struct Node **new_node_ptr)
{
    ERROR_CHECK(list         == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(new_node_ptr == NULL, ERROR_NULL_PTR);

    int list_insert_err = listInsertBefore(list, FICT_NODE->next, arg, new_node_ptr);
    ERROR_CHECK(list_insert_err, ERROR_LIST_INSERT_BEFORE);

    return SUCCESS;
}

int listPopFront(struct List *list, list_elem_t *arg)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(arg  == NULL, ERROR_NULL_PTR);

    int remove_err = listRemove(list, FICT_NODE->next, arg);
    ERROR_CHECK(remove_err, ERROR_LIST_REMOVE);

    return SUCCESS;
}

int listPopBack(struct List *list, list_elem_t *arg)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(arg  == NULL, ERROR_NULL_PTR);

    int remove_err = listRemove(list, FICT_NODE->prev, arg);
    ERROR_CHECK(remove_err, ERROR_LIST_REMOVE);

    return SUCCESS;
}

int listInsertAfter(struct List *list, struct Node *node_ptr,
                      list_elem_t arg, struct Node **new_node_ptr)
{
    ERROR_CHECK(list          == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(node_ptr      == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(new_node_ptr  == NULL, ERROR_NULL_PTR);

    VERIFICATION();

    struct Node *curr_node = (struct Node *) calloc(1, sizeof(struct Node));
    ERROR_CHECK(curr_node == NULL, ERROR_CALLOC);

    curr_node->value = arg;
    curr_node->prev  = node_ptr;

    node_ptr->next->prev = curr_node;

    curr_node->next = node_ptr->next;
    node_ptr->next  = curr_node;

    *new_node_ptr = curr_node;

    list->size += 1;

    VERIFICATION();    

    return SUCCESS;
}

int listInsertBefore(struct List *list, struct Node *node_ptr,
                       list_elem_t arg, struct Node **new_node_ptr)
{
    ERROR_CHECK(list          == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(node_ptr      == NULL, ERROR_NULL_INDX);
    ERROR_CHECK(new_node_ptr  == NULL, ERROR_NULL_PTR);

    VERIFICATION();

    struct Node *curr_node = (struct Node *) calloc(1, sizeof(struct Node));
    ERROR_CHECK(curr_node == NULL, ERROR_CALLOC);

    curr_node->value = arg;
    curr_node->prev  = node_ptr->prev;

    node_ptr->prev->next = curr_node;

    curr_node->next = node_ptr;
    node_ptr->prev = curr_node;

    *new_node_ptr = curr_node;

    list->size += 1;

    VERIFICATION();    

    return SUCCESS;
}

int listRemove(struct List *list, struct Node *node_ptr,
               list_elem_t *arg)
{
    ERROR_CHECK(list     == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(node_ptr == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(arg      == NULL, ERROR_NULL_PTR);

    VERIFICATION();

    ERROR_CHECK(list->size == 0, ERROR_INCORRECT_OPERATION);

    *arg = node_ptr->value;

    node_ptr->next->prev = node_ptr->prev;
    node_ptr->prev->next = node_ptr->next;

    free(node_ptr);

    list->size -= 1;

    VERIFICATION();
    
    return SUCCESS;
}

int list_Translate_Logical_Position_To_Physical_Adress(struct List *list,
                                                       unsigned int logic_indx, 
                                                       struct Node **phys_indx)
{
    ERROR_CHECK(list      == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(logic_indx == 0,    ERROR_NULL_INDX);
    ERROR_CHECK(phys_indx  == NULL, ERROR_NULL_PTR);

    VERIFICATION();

    struct Node *curr_node = FICT_NODE->next;
    for (unsigned int index = 1; index < logic_indx; index++)
        curr_node = CURR_NODE->next;

    *phys_indx = curr_node;

    VERIFICATION();

    return SUCCESS;
}

int listVerification(struct List *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    int list_check_err = listCheckError(list);
    ERROR_CHECK(list_check_err, ERROR_LIST_CHECK_ERROR);
    
    if (list->error != 0)
        LISTDUMP(list, "ERROR IN VERIFICATION");

    return SUCCESS;
}

int listCheckError(struct List *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    INSPECT_LIST(FICT_NODE       == NULL,        ERROR_CODE_NULL_FICT_NODE);
    INSPECT_LIST(FICT_NODE->next == NULL,        ERROR_CODE_NULL_HEAD);
    INSPECT_LIST(FICT_NODE->prev == NULL,        ERROR_CODE_NULL_TAIL);
    INSPECT_LIST(FICT_NODE->next == FICT_NODE->prev &&
                      list->size >=  3,          ERROR_CODE_HEAD_IS_TAIL);
    INSPECT_LIST((list->error & CODE_DEAD_LIST), ERROR_OPER_WITH_DEAD_LIST);

    return SUCCESS;
}