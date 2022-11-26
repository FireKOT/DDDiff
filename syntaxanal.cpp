#include <stdio.h>
#include <malloc.h>

#include "config.h"
#include "syntaxanal.h"


node_t *StartSyntaxAnal (const char * const str) {

    const char *s = str;
    node_t *root = GetSumExpr(&s);

    RET_ON_VAL(*s != '\0' || root == nullptr, ERR_SYNTAX_ERR, nullptr);
    return root;
}

node_t *GetSumExpr (const char **str) {

    node_t *root = GetMulExpr(str);

    while (**str == '+' || **str == '-') {

        const char op = **str;
        (*str)++;

        node_t *node = GetMulExpr(str);
        if (op == '+') {
            node_t *tmp = OpNodeCtor(VAL_OP, OP_ADD);
            tmp->left  = root;
            tmp->right = node;

            root = tmp;
        }
        else {
            node_t *tmp = OpNodeCtor(VAL_OP, OP_SUB);
            tmp->left  = root;
            tmp->right = node;

            root = tmp;
        }
    }

    return root;
}

node_t *GetMulExpr (const char **str) {

    node_t *root = GetBrackets(str);

    while (**str == '*' || **str == '/') {

        const char op = **str;
        (*str)++;

        node_t *node = GetBrackets(str);
        if (op == '*') {
            node_t *tmp = OpNodeCtor(VAL_OP, OP_MUL);
            tmp->left  = root;
            tmp->right = node;

            root = tmp;
        }
        else {
            node_t *tmp = OpNodeCtor(VAL_OP, OP_DIV);
            tmp->left  = root;
            tmp->right = node;

            root = tmp;
        }
    }

    return root;
} 

node_t *GetBrackets (const char **str) {

    node_t *root = nullptr;

    if (**str == '(') {
        (*str)++;
        root = GetSumExpr(str);
        RET_ON_VAL(**str != ')', ERR_BRACKET_SYNTAX, nullptr);

        (*str)++;
    }
    else root = GetNumber(str);

    return root;
}

node_t *GetNumber (const char **str) {

    elem_t val = 0;
    const char *sOld = *str;

    while ('0' <= **str && **str <= '9') {

        val = val * 10 + **str - '0';
        (*str)++;
    }   
    RET_ON_VAL(*str == sOld, ERR_NUM_SYNTAX, nullptr);

    return NumNodeCtor(VAL_NUM, val);
}
