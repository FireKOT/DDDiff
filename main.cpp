#include <stdio.h>

#include "diff.h"
#include "gdump.h"
#include "latex.h"
#include "syntaxanal.h"


int main () {

    InitLog("GraphLog.html");
    InitLaTex("LaTex.tex");

    char expr[] = "x^sinx";

    node_t *root = StartSyntaxAnal(expr);

    GDump(root);

    node_t *diff = Diff(root, "x");

    GDump(diff);

    CurtailConsts(diff);

    GDump(diff);

    PrintToLaTex(root, "LaTex.tex");
    MakeMeth ("LaTex.tex");

    DiffNodeDtor(root);
    DiffNodeDtor(diff);

    return 0;
}
