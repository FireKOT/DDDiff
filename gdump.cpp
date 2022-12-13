#include <stdio.h>
#include <windows.h>

#include "gdump.h"
#include "general.h"
#include "config.h"
#include "diff.h"


static int GDumpNum = 0;


void InitLog (const char *file_name) {

    FILE *gdump = fopen(file_name, "w");
    RET_ON_VAL(!gdump, ERR_INCRR_FILE, );

    char *styles = GetText("styles.txt");

    fprintf(gdump,  "%s", styles);

    FreeText(styles);

    fclose(gdump);
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
        char color[STRS] = {}, label[STRS] = {};

        if (node->val == VAL_NUM) {
            sprintf(color, "#00FF7F");
            sprintf(label, "%lg", node->data.num);
        }
        else if (node->val == VAL_VAR) {
            sprintf(color, "#fa9819");
            sprintf(label, "%s", node->data.var);
        }

        PrintGNodeCode (num, "box", color, label, output);

        return num; 
    }

    char label[STRS] = {}, op[STRS] = {};

    #define DEF_OP(name, num, design, ...)  \
            if (node->data.op == OP_##name) {    \
                sprintf(op, "%s", design);  \
            }                               \
            else
    
    #include "operators.h"
    /*else*/ {}

    #undef DEF_OP

    sprintf(label, "{ OP | %s}", op);
    PrintGNodeCode (num, "record", "#3FC391", label, output);

    if (node->left)  num = MakeGNodes(node->left, output, num + 1);
    if (node->right) num = MakeGNodes(node->right, output, num + 1);

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

    if (node->left) {
        PrintChainCode (from, num + 1, "bold", "#b804e0", 1, output);
        num = MakeChain(node->left, output, num + 1);
    }

    if (node->right) {
        PrintChainCode (from, num + 1, "bold", "#ff04e0", 1, output);
        num = MakeChain(node->right, output, num + 1);
    }

    return num;
}

void PrintGNodeCode (size_t num, const char *shape, const char *color, const char *label, FILE *output) {

    fprintf(output, "%d [shape = %s, style = \"filled, rounded\", fillcolor = \"%s\", label = \"%s\"];\n", num, shape, color, label);
}

void PrintChainCode (size_t from, size_t where, const char *style, const char *color, size_t weight, FILE *output) {

    fprintf(output, "%d -> %d [style = %s, color = \"%s\", weight = %d];\n", from, where, style, color, weight);
}