#ifndef DUMP_H_INCLUDED
#define DUMP_H_INCLUDED

#include "ptr_list.h"

int listDump(const struct List *list, const char *comment, 
             const char *file_name, int line_num, const char *func_name);

int CreateGraph(const struct List *list);
int SaveGraphPng(void);
int AddGraphPng(int graph_counter);

#endif // LIST_H_INCLUDED