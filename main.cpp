#include <stdio.h>

#include "diff.h"
#include "gdump.h"
#include "latex.h"
#include "syntaxanal.h"


void Run      (const char *file_name);
void ReadData (const char *file_name, char *expr, double *point, int *order);


int main () {

    InitLog("GraphLog.html");
    InitLaTex("LaTex.tex");

    Run("DataSet.txt");

    MakeMeth ("LaTex.tex");

    return 0;
}

void Run(const char *file_name) {

    char expr[STRS] = {};
    double point = 0;
    int order = 0;

    ReadData(file_name, expr, &point, &order);
    printf("function: %s\n", expr);
    printf("decomposition point: %lg\n", point);
    printf("decomposition order: %d\n", order);

    node_t *root = StartSyntaxAnal(expr);

    GDump(root);

    node_t *diff = Diff(root, "x");
    CurtailConsts(diff, diff, true);

    GDump(diff);

    node_t *taylor = Taylor(root, "x", point, order);

    GDump(taylor);

    printf("Tanget parametr: %lg\n", Solve(diff, "x", point)->data.num);

    DiffNodeDtor(root);
    //DiffNodeDtor(taylor);
}

void ReadData (const char *file_name, char *expr, double *point, int *order) {

    RET_ON_VAL(!file_name, ERR_INVAL_ARG, );

    FILE *input = fopen(file_name, "r");
    RET_ON_VAL(!input, ERR_INCRR_FILE, );

    int isread = 0;
    fscanf(input, "function: %[^\n] \n %n", expr, &isread);
    RET_ON_VAL(!isread, ERR_INCRR_INPUT, );

    isread = 0;
    fscanf(input, "decomposition point: %lg \n %n", point, &isread);
    RET_ON_VAL(!isread, ERR_INCRR_INPUT, );

    isread = 0;
    fscanf(input, "decomposition order: %d%n", order, &isread);
    RET_ON_VAL(!isread, ERR_INCRR_INPUT, );
}
