#include <stdio.h>
#include <malloc.h>

#include "config.h"
#include "diff.h"


static const size_t STRS = 1000;
static int GDumpNum = 0;


node_t *NumNodeCtor (int val_type, elem_t num) {

    node_t *node = (node_t*) calloc(sizeof(node_t), 1);

    node->val = val_type;
    node->op  = OP_NONE;
    node->num = num;

    return node;
}

node_t *OpNodeCtor (int val_type, int op_type) {

    node_t *node = (node_t*) calloc(sizeof(node_t), 1);

    node->val = val_type;
    node->op  = op_type;
    node->num = 0;

    return node;
}




void GraphDump (node_t *root, const char *file, const char *funk, int linenum) {

    char file_name[STRS] = {};
    sprintf(file_name, "gdump\\GraphImageCode#%d.txt", GDumpNum);
    FILE *gdump = fopen(file_name, "w");
    RET_ON_VAL(!gdump, ERR_ACC_DENi, );

    fprintf(gdump, "digraph G{\n");
    fprintf(gdump, "bgcolor = \"transparent\"\n");
    fprintf(gdump, "\n");


    MakeGNodes(root, gdump);
    fprintf(gdump, "\n");
    MakeChain(root, gdump);

    fprintf(gdump, "}\n");

    fclose(gdump);


    FILE *html = fopen("GraphLog.html", "a");

    char cmd[STRS] = {}, img_name[STRS] = {};
    sprintf(img_name, "gdump\\GraphImage%d.svg", GDumpNum);
    sprintf(cmd, "dot %s -T svg -o %s", file_name, img_name);
    system(cmd);

    fprintf(html, "<details>\n");
    fprintf(html, "<summary class = \"dump\">Dump #%d In file %s in function %s in line %d</summary>\n",GDumpNum+1, file, funk, linenum);
    fprintf(html, "<div> <img src = %s> </div>\n", img_name);
    fprintf(html, "</details>\n\n");

    fclose(html);

    GDumpNum++;
}

int MakeGNodes (node_t *node, FILE *output, size_t num) {

    if (!node) {
        return 0;
    }

    if (!node->left && !node->right) {
        char label[STRS] = {};
        sprintf(label, "%d", node->num);
        PrintGNodeCode (num, "box", "#00FF7F", label, output);

        return num; 
    }

    char label[STRS] = {}, op[STRS] = {};

    if (node->op == OP_ADD) sprintf(op, "+");
    else if (node->op == OP_SUB) sprintf(op, "-");
    else if (node->op == OP_MUL) sprintf(op, "*");
    else if (node->op == OP_DIV) sprintf(op, "/");

    sprintf(label, "{ OP | {VAL_OP | %s}}", op);
    PrintGNodeCode (num, "record", "#3FC391", label, output);

    num = MakeGNodes(node->left, output, num + 1);
    num = MakeGNodes(node->right, output, num + 1);

    return num;
}

int MakeChain (node_t *node, FILE *output, size_t num) {

    if (!node) {
        return 0;
    }

    if (!node->left && !node->right) {
        return num; 
    }

    size_t from = num;

    PrintChainCode (from, num + 1, "bold", "#7FFF00", 1, output);
    num = MakeChain(node->left, output, num + 1);

    PrintChainCode (from, num + 1, "bold", "#FF4500", 1, output);
    num = MakeChain(node->right, output, num + 1);

    return num;
}

void PrintGNodeCode (size_t num, const char *shape, const char *color, const char *label, FILE *output) {

    fprintf(output, "%d [shape = %s, style = \"filled\", fillcolor = \"%s\", label = \"%s\"];\n", num, shape, color, label);
}

void PrintChainCode (size_t from, size_t where, const char *style, const char *color, size_t weight, FILE *output) {

    fprintf(output, "%d -> %d [style = %s, color = \"%s\", weight = %d];\n", from, where, style, color, weight);
}
