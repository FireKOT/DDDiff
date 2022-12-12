#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "syntaxanal.h"
#include "config.h"
#include "vec.h"


node_t *StartSyntaxAnal (const char *expr) {

    RET_ON_VAL(!expr, ERR_INVAL_ARG, nullptr);

    vec_t vec = LexicalAnalysis(expr);
    RET_ON_VAL(!vec.data, ERR_SEG_FAULT, nullptr);

    lexnode_t *lexnodes = vec.data;
    node_t *root = GetSumExpr(&lexnodes);
    SYNTAX_ERR_PROC(!root || lexnodes->val != VAL_END, ERR_SYNTAX_ERR, expr, (--lexnodes)->pos, nullptr);

    return root;
}

node_t *GetSumExpr (lexnode_t **lexnodes) {

    node_t *root = GetMulExpr(lexnodes);
    if(!root) return nullptr;

    while ((*lexnodes)->op == OP_ADD || (*lexnodes)->op == OP_SUB) {

        const int op = (*lexnodes)->op;
        (*lexnodes)++;

        node_t *node = GetMulExpr(lexnodes);
        if (!node) return nullptr;

        if (op == OP_ADD) {
            node_t *tmp = OpNodeCtor(OP_ADD);
            tmp->left  = root;
            tmp->right = node;

            root = tmp;
        }
        else {
            node_t *tmp = OpNodeCtor(OP_SUB);
            tmp->left  = root;
            tmp->right = node;

            root = tmp;
        }
    }

    return root;
}

node_t *GetMulExpr (lexnode_t **lexnodes) {

    node_t *root = GetUnoExpr(lexnodes);
    if(!root) return nullptr;

    while ((*lexnodes)->op == OP_MUL || (*lexnodes)->op == OP_DIV) {

        const int op = (*lexnodes)->op;
        (*lexnodes)++;

        node_t *node = GetUnoExpr(lexnodes);
        if (!node) return nullptr;

        if (op == OP_MUL) {
            node_t *tmp = OpNodeCtor(OP_MUL);
            tmp->left  = root;
            tmp->right = node;

            root = tmp;
        }
        else {
            node_t *tmp = OpNodeCtor(OP_DIV);
            tmp->left  = root;
            tmp->right = node;

            root = tmp;
        }
    }

    return root;
}

node_t *GetUnoExpr (lexnode_t **lexnodes) {

    const int op = (*lexnodes)->op;
    node_t *root = nullptr;

    (*lexnodes)++;

    #define DEF_OP(name, num, design, isunary, ...)     \
            if (isunary && op == OP_##name) {           \
                __VA_ARGS__                             \
            }                                           \
            else    

    #include "operators.h"

    /*else*/ {
        (*lexnodes)--;
        root = GetPowExpr(lexnodes);
    }

    #undef DEF_OP

    return root;
}

node_t *GetPowExpr (lexnode_t **lexnodes) {

    node_t *root = GetBrackets(lexnodes);
    if (!root) return nullptr;

    if ((*lexnodes)->op == OP_POW) {
        (*lexnodes)++;

        node_t *node = GetUnoExpr(lexnodes);
        if (!node) return nullptr;

        node_t *tmp = OpNodeCtor(OP_POW);
        tmp->left  = root;
        tmp->right = node;

        root = tmp;
    }

    return root;
} 

node_t *GetBrackets (lexnode_t **lexnodes) {

    node_t *root = nullptr;

    if ((*lexnodes)->op == OP_OBRACKET) {
        (*lexnodes)++;
        root = GetSumExpr(lexnodes);
        RET_ON_VAL((*lexnodes)->op != OP_CBRACKET, ERR_BRACKET_SYNTAX, nullptr);

        (*lexnodes)++;
    }
    else if ((*lexnodes)->val == VAL_NUM) root = NumNodeCtor(((*lexnodes)++)->num);
    else if ((*lexnodes)->val == VAL_VAR) root = VarNodeCtor(((*lexnodes)++)->var);

    return root;
}


vec_t LexicalAnalysis (const char * const str) {

    vec_t err {
        .data     = nullptr,
        .capacity = 0,
        .size     = 0,
    };

    RET_ON_VAL(!str, ERR_INVAL_ARG, err);

    const char *s = str;
    vec_t lexnodes = VecCtor();

    while (*s != '\0') {
        if (*s == ' ' || *s == '\t') {
            s++;
            continue;
        }

        int op_t = OP_NONE;
        int disp = 0;

        #define DEF_OP(name, num, design, ...)                                      \
                if (!strncmp(s, design, strlen(design)) && OP_##name != OP_NONE) {  \
                    op_t = OP_##name;                                               \
                    disp = strlen(design);                                          \
                }                                                                   \
                else

        #include "operators.h"
        /*else*/ {}

        #undef DEF_OP

        if (op_t != OP_NONE) {
            lexnode_t node {
                .val = VAL_OP,
                .op  = op_t,
                .pos = s - str,
                .num = 0,
                .var = {},
            };

            PushBack(&lexnodes, node);

            s += disp;
        }
        else {
            int pos = 0;
            elem_t num = 0;
            char var[STRS] = {};
            sscanf(s, "%lf%n", &num, &pos);

            lexnode_t node {
                .val = VAL_NONE,
                .op  = OP_NONE,
                .pos = s - str,
                .num = num,
                .var = {},
            };

            if (!pos) {
                sscanf(s, "%[^ +-*/^()]%n", var, &pos);

                SYNTAX_ERR_PROC(!pos, ERR_NUM_SYNTAX, str, s - str, err);
                SYNTAX_ERR_PROC(strlen(var) >= MAX_VAR_NAME_LEN, ERR_VAR_SYNTAX, str, s - str, err);

                node.val = VAL_VAR;
                strncpy(node.var, var, MAX_VAR_NAME_LEN);
            }
            else {
                node.val = VAL_NUM;
            }

            PushBack(&lexnodes, node);

            s += pos;
        }
    }

    lexnode_t end {
        .val = VAL_END,
        .op  = OP_NONE,
        .pos = 0,
        .num = 0,
        .var = {},
    };

    PushBack(&lexnodes, end);

    return lexnodes;
}
