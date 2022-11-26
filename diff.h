#ifndef _DIFF_H
#define _DIFF_H


#define GDump(tree) GraphDump(tree, __FILE__, __PRETTY_FUNCTION__, __LINE__);


typedef int elem_t;


enum VAL_T {

    VAL_OP  = 1,
    VAL_NUM = 2,
    VAL_VAR = 3,
};

enum OP_T {

    OP_NONE = 0,
    OP_ADD  = 1,
    OP_SUB  = 2,
    OP_MUL  = 3,
    OP_DIV  = 4,
    OP_POW  = 5,
};


struct node_t {

    int val, op;

    elem_t num;

    node_t *left, *right;
};


node_t *NumNodeCtor (int val_type, elem_t num);
node_t *OpNodeCtor  (int val_type, int op_type);

void GraphDump      (node_t *root, const char *file, const char *funk, int linenum);
int  MakeGNodes     (node_t *node, FILE *output, size_t num = 0);
int  MakeChain      (node_t *node, FILE *output, size_t num = 0);
void PrintGNodeCode (size_t num, const char *shape, const char *color, const char *label, FILE *output);
void PrintChainCode (size_t from, size_t where, const char *style, const char *color, size_t weight, FILE *output);


#endif