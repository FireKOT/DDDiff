#include <stdio.h>

#include "diff.h"
#include "syntaxanal.h"


int main () {

    char expr[] = "15+18*(17+19)";

    node_t *root = StartSyntaxAnal(expr);
    GDump(root);

    return 0;
}
