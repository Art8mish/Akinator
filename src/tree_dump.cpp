#include "../include/akinator.h"

#define PRINT_ERROR(err)                            \
            do                                      \
            {                                       \
                if (tree->error & err)              \
                    fprintf(dump_f, "%s\n", #err);  \
            } while(false)

int treeDump(const struct Tree *tree, const char *comment, 
             const char *file_name, int line_num, const char *func_name)
{
    ERROR_CHECK(tree == NULL, ERROR_NULL_PTR);

    ERROR_CHECK(file_name == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(func_name == NULL, ERROR_NULL_PTR);

    FILE *dump_f = fopen(TREE_HTM_FILE, "a");
    ERROR_CHECK(dump_f == NULL, ERROR_OPENING_FILE);

    fprintf(dump_f, "<b><h2>%s</h2></b>\n", comment);
    fprintf(dump_f, "\nTree DUMP at %s(line: %d) at %s\n", file_name, line_num, func_name);
    fprintf(dump_f, "Tree[%p]\n", tree);
    fprintf(dump_f, "{\n");
    fprintf(dump_f, "\tsize = %d\n", tree->size);

    fprintf(dump_f, "\troot = %d\n", (int)tree->root);

    fprintf(dump_f, "}\n\n");

    if (tree->error != 0)
    {
        fprintf(dump_f, "ERROR in Tree (%x):\n", tree->error);
        PRINT_ERROR(ERROR_CODE_NULL_TREE);
        PRINT_ERROR(CODE_DEAD_TREE);
    }

    int fclose_err = fclose(dump_f);
    ERROR_CHECK(fclose_err, ERROR_CLOSING_FILE);

    int create_graph_err = CreateTreeGraph(tree);
    ERROR_CHECK(create_graph_err, ERROR_CREATE_GRAPH);

    fprintf(dump_f, "<hr>\n");

    fprintf(dump_f, "\nEnd of DUMP...\n\n");

    return SUCCESS;
}

int CreateTreeGraph(const struct Tree *tree)
{
    ERROR_CHECK(tree == NULL, ERROR_NULL_PTR);

    FILE *graph_f = fopen(TREE_GRAPH_FILE, "w");
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
                        "label=\"{<f0> %d|" TREE_SPECIFIER "|{<f2>left_yes %d | <f1>rght_no %d}}\"]\n",
                        (int)curr_node, fill_color, color, (int)curr_node, curr_node->value, (int)curr_node->left_yes, (int)curr_node->rght_no);

    if (curr_node->left_yes != NULL)
        fprintf(graph_f, "\t%d:<f2>:s->%d:n[weight = 1, constraint=true, style=\"dashed\", label=\"yes\"]\n", (int)curr_node, (int)curr_node->left_yes);

    if (curr_node->rght_no != NULL)
        fprintf(graph_f, "\t%d:<f1>:s->%d:n[weight = 1, constraint=true, style=\"dashed\", label=\"no\"]\n", (int)curr_node, (int)curr_node->rght_no);

    if (curr_node->rght_no != NULL)
    {
        int create_nodes_err = CreateTreeNodes(curr_node->rght_no, graph_f);
        FILE_ERROR_CHECK(create_nodes_err, ERROR_CREATE_NODES, graph_f);
    }

    if (curr_node->left_yes != NULL)
    {
        int create_nodes_err = CreateTreeNodes(curr_node->left_yes, graph_f);
        FILE_ERROR_CHECK(create_nodes_err, ERROR_CREATE_NODES, graph_f);
    }

    return SUCCESS;
}

int SaveTreeGraphPng(void)
{
    static int graph_counter = 1;

    char command[100] = "dot tree_graph.gv -Tpng -o images/tree_graph";
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
    FILE *htm_f = fopen(TREE_HTM_FILE, "a");
    ERROR_CHECK(htm_f == NULL, ERROR_OPENING_FILE);
    
    fprintf(htm_f, "<img width=400 src=\"../images/tree_graph%d.png\"><br><hr>\n", graph_counter);
    
    int fclose_err = fclose(htm_f);
    ERROR_CHECK(fclose_err, ERROR_CLOSING_FILE);

    return SUCCESS;
}   