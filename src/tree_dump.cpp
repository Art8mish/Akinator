#include "../include/tree.h"

#define PRINT_ERROR(err)                            \
            do                                      \
            {                                       \
                if (tree->error & err)              \
                    fprintf(dump_f, "%s\n", #err);  \
            } while(false)

int TreeDump(const struct Tree *tree, const char *comment, bool debug,
             const char *file_name, int line_num, const char *func_name)
{
    ERROR_CHECK(tree == NULL, ERROR_NULL_PTR);

    FILE *dump_f = fopen(TREE_HTM_PATH, "a");
    ERROR_CHECK(dump_f == NULL, ERROR_OPENING_FILE);

    if (debug)
    {
        if (comment != NULL)
            fprintf(dump_f, "<b><h2>%s</h2></b>\n", comment);

        if (file_name != NULL)
            fprintf(dump_f, "\nTree DUMP at %s ", file_name);
        
        if (line_num != 0)
            fprintf(dump_f, "(line: %d) ", line_num);

        if (func_name != NULL)
            fprintf(dump_f, "at %s\n", func_name);
        
        fprintf(dump_f, "Tree[%p]\n", tree);
        fprintf(dump_f, "{\n");
        fprintf(dump_f, "\tsize = %d\n", tree->size);

        fprintf(dump_f, "\troot = %d\n", (int)tree->root);

        fprintf(dump_f, "}\n\n");
    

        if (tree->error != 0)
            fprintf(dump_f, "ERROR in Tree (%x):\n", tree->error);
    }

    int create_graph_err = CreateTreeGraph(tree);
    ERROR_CHECK(create_graph_err, ERROR_CREATE_GRAPH);

    fprintf(dump_f, "<hr>\n");

    int fclose_err = fclose(dump_f);
    ERROR_CHECK(fclose_err, ERROR_CLOSING_FILE);

    return SUCCESS;
}

int CreateTreeGraph(const struct Tree *tree)
{
    ERROR_CHECK(tree == NULL, ERROR_NULL_PTR);

    FILE *graph_f = fopen(TREE_GRAPH_PATH, "w");
    ERROR_CHECK(graph_f == NULL, ERROR_OPENING_FILE);

    fprintf(graph_f, "digraph tree{\n\trankdir=TB;\nspline = ortho;\n");

    if (tree->root != NULL)
    {
        int create_nodes_err = CreateTreeNodes(tree->root, graph_f);
        FILE_ERROR_CHECK(create_nodes_err, ERROR_CREATE_NODES, graph_f);
    }

    fprintf(graph_f, "}\n");
    
    int fclose_err = fclose(graph_f);
    ERROR_CHECK(fclose_err, ERROR_CLOSING_FILE);

    SaveTreeGraphPng();

    return SUCCESS;
}

int CreateTreeNodes(const struct TreeNode *curr_node, FILE *graph_f)
{
    ERROR_CHECK(graph_f   == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(curr_node == NULL, ERROR_NULL_PTR);

    const char *color = "red3";
    const char *fill_color = "peachpuff";

    fprintf(graph_f, "\t%d[style=\"filled\",fillcolor=\"%s\"shape=record,color=%s,"
                        "label=\"{{<f0> 0x%d |<f3>parent 0x%d}|" TREE_SPECIFIER "|{<f2>left_yes 0x%d | <f1>rght_no 0x%d}}\"]\n",
                        (int)curr_node, fill_color, color, (int)curr_node, (int)curr_node->parent, 
                        curr_node->value, (int)curr_node->left, (int)curr_node->right);

    if (curr_node->left  != NULL)
        fprintf(graph_f, "\t%d:<f2>:s->%d:<f0>:n[weight = 1, constraint=true, style=\"dashed\", label=\"yes\"]\n", 
                         (int)curr_node, (int)curr_node->left);

    if (curr_node->right != NULL)
        fprintf(graph_f, "\t%d:<f1>:s->%d:<f0>:n[weight = 1, constraint=true, style=\"dashed\", label=\"no\"]\n", 
                         (int)curr_node, (int)curr_node->right);

    if (curr_node->right != NULL)
    {
        int create_nodes_err = CreateTreeNodes(curr_node->right, graph_f);
        FILE_ERROR_CHECK(create_nodes_err, ERROR_CREATE_NODES, graph_f);
    }

    if (curr_node->left  != NULL)
    {
        int create_nodes_err = CreateTreeNodes(curr_node->left, graph_f);
        FILE_ERROR_CHECK(create_nodes_err, ERROR_CREATE_NODES, graph_f);
    }

    return SUCCESS;
}

int SaveTreeGraphPng(void)
{
    static int graph_counter = 1;

    char command[MAX_COMMAND_SIZE] = "dot tree_graph.gv -Tpng -o images/tree_graph";
    char com_part[] = ".png";

    sprintf(command, "%s%d%s",command, graph_counter, com_part);

    system(command);

    int add_graph_png_err = AddTreeGraphPng(graph_counter);
    ERROR_CHECK(add_graph_png_err, ERROR_ADD_GRAPH);

    graph_counter++;

    return SUCCESS;
}

int AddTreeGraphPng(int graph_counter)
{
    FILE *htm_f = fopen(TREE_HTM_PATH, "a");
    ERROR_CHECK(htm_f == NULL, ERROR_OPENING_FILE);
    
    fprintf(htm_f, "<img width=400 src=\"../images/tree_graph%d.png\"><br>\n", graph_counter);
    
    int fclose_err = fclose(htm_f);
    ERROR_CHECK(fclose_err, ERROR_CLOSING_FILE);

    return SUCCESS;
}   