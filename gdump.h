#ifndef _GDUMP_H
#define _GDUMP_H

#include <stdio.h>

#include "diff.h"


void InitLog        (const char *file_name);
void GraphDump      (node_t *root, const char *file, const char *funk, int linenum);
int  MakeGNodes     (node_t *node, FILE *output, size_t num = 1);
int  MakeChain      (node_t *node, FILE *output, size_t num = 1);
void PrintGNodeCode (size_t num, const char *shape, const char *color, const char *label, FILE *output);
void PrintChainCode (size_t from, size_t where, const char *style, const char *color, size_t weight, FILE *output);


#endif