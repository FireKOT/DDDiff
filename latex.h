#ifndef LATEX_H
#define LATEX_H


#include <stdio.h>

#include "diff.h"


void InitLaTex    (const char *file_name);
void PrintToLaTex (node_t *root, const char *file_name);
void PrintTree    (node_t *node, FILE *output);
void MakeMeth     (const char *file_name);


#endif