#ifndef _DIFF_H
#define _DIFF_H


#include "config.h"


#define GDump(tree) GraphDump(tree, __FILE__, __PRETTY_FUNCTION__, __LINE__);


const size_t MAX_VAR_NAME_LEN = 8;


typedef double elem_t;


enum VAL_T {

    VAL_NONE = 0,
    VAL_OP   = 1,
    VAL_NUM  = 2,
    VAL_VAR  = 3,
    VAL_END  = 4,
};

#define DEF_OP(name, num, ...) OP_##name = num,

enum OP_T {

    #include "operators.h"
};

#undef DEF_OP

struct node_t {

    int val;

    union nodedata{
        int op;
        elem_t num;
        char var[MAX_VAR_NAME_LEN];
    } data;

    node_t *left, *right;
};

struct lexnode_t {

    int val, op;
    int pos;

    elem_t num;
    char var[MAX_VAR_NAME_LEN];
};

node_t *OpNodeCtor       (int op, node_t *left = nullptr, node_t *right = nullptr);
node_t *NumNodeCtor      (elem_t num);
node_t *VarNodeCtor      (const char *var);
void    DiffNodeDtor     (node_t *node);
void    FreeOnlyThisNode (node_t *node);
node_t *Diff             (node_t *expr, const char *diffvar);
node_t *Taylor           (node_t *root, const char *diffvar, double dot, int ord);
int     Factor           (int num);   
node_t *Solve            (node_t *root, const char *var, const double num);
void    Replace          (node_t *root, const char *var, const double num);
node_t *CopyNode         (node_t *node);

int CurtailConsts (node_t *root, node_t *treepeak = nullptr, int isprint = false);
int CurtailAdd    (node_t *root, node_t *treepeak, int *ischanged, int isprint);
int CurtailSub    (node_t *root, node_t *treepeak, int *ischanged, int isprint);
int CurtailMul    (node_t *root, node_t *treepeak, int *ischanged, int isprint);
int CurtailDiv    (node_t *root, node_t *treepeak, int *ischanged, int isprint);
int CurtailPow    (node_t *root, node_t *treepeak, int *ischanged, int isprint);
int CurtailLn     (node_t *root, node_t *treepeak, int *ischanged, int isprint);
int CurtailSin    (node_t *root, node_t *treepeak, int *ischanged, int isprint);
int CurtailCos    (node_t *root, node_t *treepeak, int *ischanged, int isprint);
int CurtailTg     (node_t *root, node_t *treepeak, int *ischanged, int isprint);
int CurtailCtg    (node_t *root, node_t *treepeak, int *ischanged, int isprint);


#endif