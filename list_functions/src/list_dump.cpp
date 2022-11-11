
#include "../include/ptr_list.h"

#define PRINT_ERROR(err)                            \
            do                                      \
            {                                       \
                if (list->error & err)              \
                    printf("%s\n", #err);           \
            } while(false)



int listDump(const struct List *list, const char *comment, 
             const char *file_name, int line_num, const char *func_name)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    ERROR_CHECK(file_name == NULL, ERROR_NULL_PTR);
    ERROR_CHECK(func_name == NULL, ERROR_NULL_PTR);

    FILE *dump_f = fopen(HTM_FILE, "a");
    ERROR_CHECK(dump_f == NULL, ERROR_OPENING_FILE);

    fprintf(dump_f, "<b><h2>%s</h2></b>\n", comment);
    fprintf(dump_f, "\nList DUMP at %s(line: %d) at %s\n", file_name, line_num, func_name);
    fprintf(dump_f, "List[%p]\n", list);
    fprintf(dump_f, "{\n");
    fprintf(dump_f, "\tsize     = %d\n\n", list->size);

    fprintf(dump_f, "\thead = %p\n",   list->fict_node->next);
    fprintf(dump_f, "\ttail = %p\n",   list->fict_node->prev);

    fprintf(dump_f, "}\n\n");

    if (list->error != 0)
    {
        fprintf(dump_f, "ERROR in List (%x):\n", list->error);
        PRINT_ERROR(ERROR_CODE_NULL_HEAD);
        PRINT_ERROR(ERROR_CODE_NULL_TAIL);
        PRINT_ERROR(ERROR_CODE_HEAD_IS_TAIL);
        PRINT_ERROR(ERROR_CODE_NULL_FICT_NODE);
        PRINT_ERROR(ERROR_OPER_WITH_DEAD_LIST);
        PRINT_ERROR(CODE_DEAD_LIST);
    }

    else
    {
        struct Node *curr_node = list->fict_node;

        fprintf(dump_f, "    Nodes[]\n");
        fprintf(dump_f, "    {\n");
        fprintf(dump_f, "        indx         value    next       prev\n");
        for (unsigned int i = 0; i < list->size + 1; i++)
        {
            fprintf(dump_f, "        [%-6p]   " LIST_SPECIFIER "   %-6p   %-6p", 
                            curr_node, curr_node->value, curr_node->next, curr_node->prev);
            if (curr_node == list->fict_node->next)
                fprintf(dump_f, "  <--- HEAD");
            if (curr_node == list->fict_node->prev)
                fprintf(dump_f, "  <--- TAIL");
            fprintf(dump_f, "\n");

            curr_node = curr_node->next;
        }
        fprintf(dump_f, "    }\n");
    }

    int fclose_err = fclose(dump_f);
    ERROR_CHECK(fclose_err, ERROR_CLOSING_FILE);

    int create_graph_err = CreateGraph(list);
    ERROR_CHECK(create_graph_err, ERROR_CREATE_GRAPH);

    fprintf(dump_f, "<hr>\n");

    fprintf(dump_f, "\nEnd of DUMP...\n\n");

    return SUCCESS;
}

int CreateGraph(const struct List *list)
{
    ERROR_CHECK(list == NULL, ERROR_NULL_PTR);

    FILE *graph_f = fopen(GRAPH_FILE, "w");
    ERROR_CHECK(graph_f == NULL, ERROR_OPENING_FILE);

    struct Node *curr_node = list->fict_node;

    fprintf(graph_f, "digraph list{\n\trankdir=LR;\nspline = ortho;\n");
    fprintf(graph_f, "\t%d[shape=record, style=\"filled\", fillcolor=\"darkolivegreen2\", color=seagreen,"
                     "label=\"<f0>%d|data " LIST_SPECIFIER "|{<f2>prev %d | <f1>next %d}\"]\n",
                     (int)curr_node, (int)curr_node, curr_node->value, (int)curr_node->prev, (int)curr_node->next);

    for (unsigned int i = 0; i < list->size + 1; i++)
    {
        fprintf(graph_f, "\t%d -> %d [style=\"invis\", weight=1000,constraint=true]\n", (int)curr_node, (int)curr_node->next);
        curr_node = curr_node->next;
    }
    
    const char *color = "red3";
    const char *fill_color = "peachpuff";
    
    for (unsigned int i = 0; i < list->size; i++)
    {
        curr_node = curr_node->next;
        fprintf(graph_f, "\t%d[style=\"filled\",fillcolor=\"%s\"shape=record,color=%s,"
                         "label=\"<f0> %d|data " LIST_SPECIFIER "|{<f2>prev %d | <f1>next %d}\"]\n",
                         (int)curr_node, fill_color, color, (int)curr_node, curr_node->value, (int)curr_node->prev, (int)curr_node->next);

        fprintf(graph_f, "\t%d:<f1>:s->%d:<f2>:s[dir=both,weight = 1, constraint=true,style=\"dashed\"]\n", (int)curr_node, (int)curr_node->next);
        
    }

    fprintf(graph_f, "\t%d:<f1>:s->%d:<f2>:s[dir=both,weight = 1, constraint=true,style=\"dashed\"]\n", (int)list->fict_node, (int)list->fict_node->next);

    fprintf(graph_f, "\nHEAD[style=\"filled\",fillcolor=\"skyblue\",label=\"HEAD = %d\"]", (int)list->fict_node->next);
    fprintf(graph_f, "\tHEAD->%d:<f0>{rank = same; %d; HEAD;}\n", (int)list->fict_node->next, (int)list->fict_node->next);

    fprintf(graph_f, "\nTAIL[style=\"filled\",fillcolor=\"skyblue\",label=\"TAIL = %d\"]", (int)list->fict_node->prev);
    fprintf(graph_f, "\tTAIL->%d:<f0>{rank = same; %d; TAIL;}; \n", (int)list->fict_node->prev, (int)list->fict_node->prev);

    fprintf(graph_f, "}\n");
    
    int fclose_err = fclose(graph_f);
    ERROR_CHECK(fclose_err, ERROR_CLOSING_FILE);

    SaveGraphPng();

    return SUCCESS;
}

int SaveGraphPng(void)
{
    static int graph_counter = 1;

    char command[100] = "dot graph.gv -Tpng -o images/graph";
    char com_part[] = ".png";

    sprintf(command, "%s%d%s",command, graph_counter, com_part);
    graph_counter++;

    system(command);

    int add_graph_png_err = AddGraphPng(graph_counter);
    ERROR_CHECK(add_graph_png_err, ERROR_ADD_GRAPH);

    return SUCCESS;
}

int AddGraphPng(int graph_counter)
{
    FILE *htm_f = fopen(HTM_FILE, "a");
    ERROR_CHECK(htm_f == NULL, ERROR_OPENING_FILE);
    
    fprintf(htm_f, "<img width=1800 src=\"../images/graph%d.png\"><br><hr>\n", graph_counter);
    
    int fclose_err = fclose(htm_f);
    ERROR_CHECK(fclose_err, ERROR_CLOSING_FILE);

    return SUCCESS;
}   