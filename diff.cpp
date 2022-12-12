#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <math.h>

#include "diff.h"
#include "gdump.h"
#include "latex.h"
#include "config.h"
#include "general.h"


node_t *OpNodeCtor (int op, node_t *left, node_t *right) {

    node_t *node = (node_t*) calloc(sizeof(node_t), 1);

    node->val     = VAL_OP;
    node->data.op = op;
    node->left  = left;
    node->right = right;

    return node;
}

node_t *NumNodeCtor (elem_t num, node_t *left, node_t *right) {

    node_t *node = (node_t*) calloc(sizeof(node_t), 1);

    node->val      = VAL_NUM;
    node->data.num = num;
    node->left  = left;
    node->right = right;

    return node;
}

node_t *VarNodeCtor (char *var, node_t *left, node_t *right) {

    node_t *node = (node_t*) calloc(sizeof(node_t), 1);

    node->val = VAL_VAR;
    strncpy (node->data.var, var, MAX_VAR_NAME_LEN);
    node->left  = left;
    node->right = right;

    return node;
}

void DiffNodeDtor (node_t *node) {

    if (!node) return;

    node->val = VAL_NONE;
    node->data = {};

    DiffNodeDtor(node->left);
    DiffNodeDtor(node->right);

    node->left  = nullptr;
    node->right = nullptr;

    free(node);
}

void FreeOnlyThisNode (node_t *node) {

    if (!node) return;

    node->val = VAL_NONE;
    node->data = {};

    node->left  = nullptr;
    node->right = nullptr;

    free(node);
}

node_t *Diff (node_t *expr, const char *diffvar) {

    int op = 0;
    if (expr->val == VAL_OP) op = expr->data.op;
    else op = OP_NONE;

    switch (op){
        case OP_NONE:{
            if (expr->val == VAL_NUM || (expr->val == VAL_VAR && strcmp(expr->data.var, diffvar)))
                return NumNodeCtor(0);
            else if (expr->val == VAL_VAR && !strcmp(expr->data.var, diffvar)) 
                return NumNodeCtor(1);

            break;
        }

        case OP_ADD:{
            return OpNodeCtor(OP_ADD, Diff(expr->left, diffvar), Diff(expr->right, diffvar));

            break;
        }

        case OP_SUB:{
            return OpNodeCtor(OP_SUB, Diff(expr->left, diffvar), Diff(expr->right, diffvar));

            break;
        }

        case OP_MUL:{
            return OpNodeCtor(OP_ADD,  OpNodeCtor(OP_MUL, Diff(expr->left, diffvar), CopyNode(expr->right)),\
                                       OpNodeCtor(OP_MUL, CopyNode(expr->left),      Diff(expr->right, diffvar)));

            break;
        }

        case OP_DIV:{
            return OpNodeCtor(OP_DIV, OpNodeCtor(OP_SUB, OpNodeCtor(OP_MUL, Diff(expr->left, diffvar), CopyNode(expr->right)),\
                                                         OpNodeCtor(OP_MUL, CopyNode(expr->left),      Diff(expr->right, diffvar))),\
                                      OpNodeCtor(OP_POW, CopyNode(expr->right), NumNodeCtor(2)));

            break;
        }

        case OP_LN:{
            return OpNodeCtor(OP_DIV, Diff(expr->right, diffvar), CopyNode(expr->right));

            break;
        }

        case OP_POW:{

            node_t *mulnode = OpNodeCtor(OP_MUL, CopyNode(expr->right), OpNodeCtor(OP_LN, nullptr, CopyNode(expr->left)));

            node_t *node = OpNodeCtor(OP_MUL, CopyNode(expr), Diff(mulnode, diffvar));

            DiffNodeDtor(mulnode);

            return node;

            break;
        }

        case OP_SIN:{
            return OpNodeCtor(OP_MUL, OpNodeCtor(OP_COS, nullptr, CopyNode(expr->right)), Diff(expr->right, diffvar));

            break;
        }

        case OP_COS:{

            return OpNodeCtor(OP_MUL, OpNodeCtor(OP_MUL, NumNodeCtor(-1), OpNodeCtor(OP_SIN, nullptr, CopyNode(expr->right))),\
                                      Diff(expr->right, diffvar));

            break;
        }

        case OP_TG:{
            return OpNodeCtor(OP_DIV, Diff(expr->right, diffvar),\
                                      OpNodeCtor(OP_POW, OpNodeCtor(OP_COS, nullptr, CopyNode(expr->right)), NumNodeCtor(2)));

            break;
        }

        case OP_CTG:{
            return OpNodeCtor(OP_DIV, OpNodeCtor(OP_MUL, NumNodeCtor(-1), Diff(expr->right, diffvar)),\
                                      OpNodeCtor(OP_POW, OpNodeCtor(OP_SIN, nullptr, CopyNode(expr->right)), NumNodeCtor(2)));

            break;
        }

        case OP_ARCSIN:{
            return OpNodeCtor(OP_MUL, OpNodeCtor(OP_DIV,NumNodeCtor(1),\
                                                        OpNodeCtor(OP_POW, OpNodeCtor(OP_SUB, NumNodeCtor(1),\
                                                                                              OpNodeCtor(OP_POW, CopyNode(expr->right), NumNodeCtor(2))),\
                                                                           NumNodeCtor(0.5))),\
                                      Diff(expr->right, diffvar));

            break;
        }

        case OP_ARCCOS:{
            return OpNodeCtor(OP_MUL, OpNodeCtor(OP_DIV, NumNodeCtor(-1),\
                                                         OpNodeCtor(OP_POW, OpNodeCtor(OP_SUB, NumNodeCtor(1),\
                                                                                               OpNodeCtor(OP_POW, CopyNode(expr->right), NumNodeCtor(2))),\
                                                                            NumNodeCtor(0.5))),\
                                      Diff(expr->right, diffvar));

            break;
        }

        case OP_ARCTG:{
            return OpNodeCtor(OP_MUL, OpNodeCtor(OP_DIV, NumNodeCtor(1),\
                                                         OpNodeCtor(OP_ADD, NumNodeCtor(1),\
                                                                            OpNodeCtor(OP_POW, CopyNode(expr->right), NumNodeCtor(2)))),\
                                      Diff(expr->right, diffvar));

            break;
        }

        case OP_ARCCTG:{
            return OpNodeCtor(OP_MUL, OpNodeCtor(OP_DIV, NumNodeCtor(-1),\
                                                         OpNodeCtor(OP_ADD, NumNodeCtor(1),\
                                                                            OpNodeCtor(OP_POW, CopyNode(expr->right), NumNodeCtor(2)))),\
                                      Diff(expr->right, diffvar));

            break;
        }
        
        default:
            RET_ON_VAL(true, ERR_DIFF_ERR, nullptr);
            break;
    }

    return nullptr;
}

node_t *CopyNode (node_t *node) {

    if (!node) return nullptr;

    node_t *newnode = nullptr;

    if (node->val == VAL_OP) newnode = OpNodeCtor(node->data.op);
    else if (node->val == VAL_NUM) newnode = NumNodeCtor(node->data.num);
    else if (node->val == VAL_VAR) newnode = VarNodeCtor(node->data.var);

    newnode->left  = CopyNode(node->left);
    newnode->right = CopyNode(node->right);

    return newnode;
}

int CurtailConsts (node_t *root) {

    RET_ON_VAL(!root, ERR_INVAL_ARG, -1);

    if (root->val == VAL_NUM) return 1;
    else if (root->val == VAL_VAR) return 0;

    switch (root->data.op){

        case OP_ADD:
            return CurtailAdd(root);
            break;

        case OP_SUB:
            return CurtailSub(root);
            break;

        case OP_MUL:
            return CurtailMul(root);
            break;

        case OP_DIV:
            return CurtailDiv(root);
            break;

        case OP_POW:
            return CurtailPow(root);
            break;
        
        default:
            CurtailConsts(root->right);
            break;
    }

    return 0;
}

int CurtailAdd(node_t *root) {

    int curleft  = CurtailConsts(root->left);
    int curright = CurtailConsts(root->right);

    if (curleft == 1 && curright == 1) {

        node_t *node = NumNodeCtor(root->left->data.num + root->right->data.num);
        *root = *node;

        DiffNodeDtor(node);
        DiffNodeDtor(root->left);
        DiffNodeDtor(root->right);

        return 1;
    }
    else if (curleft == 1 || curright == 1) {

        if (curleft && IsEqDoubleNums(root->left->data.num, 0)) {

            node_t node = *root->right;

            DiffNodeDtor    (root->left);
            FreeOnlyThisNode(root->right);

            *root = node;
        }
        else if (curright && IsEqDoubleNums(root->right->data.num, 0)) {

            node_t node = *root->left;

            FreeOnlyThisNode(root->left);
            DiffNodeDtor    (root->right);

            *root = node;
        }
    }

    return 0;
}

int CurtailSub (node_t *root) {

    int curleft  = CurtailConsts(root->left);
    int curright = CurtailConsts(root->right);

    if (curleft == 1 && curright == 1) {

        node_t *node = NumNodeCtor(root->left->data.num - root->right->data.num);
        *root = *node;

        DiffNodeDtor(node);
        DiffNodeDtor(root->left);
        DiffNodeDtor(root->right);

        return 1;
    }
    else if (curleft == 1 || curright == 1) {

        if (curleft && IsEqDoubleNums(root->left->data.num, 0)) {

            node_t node = *root->right;

            DiffNodeDtor    (root->left);
            FreeOnlyThisNode(root->right);

            *root = node;
        }
        else if (curright && IsEqDoubleNums(root->right->data.num, 0)) {

            node_t node = *root->left;

            FreeOnlyThisNode(root->left);
            DiffNodeDtor    (root->right);

            *root = node;
        }
    }

    return 0;
}

int CurtailMul (node_t *root) {

    int curleft  = CurtailConsts(root->left);
    int curright = CurtailConsts(root->right);

    if (curleft == 1 && curright == 1) {

        node_t *node = NumNodeCtor(root->left->data.num * root->right->data.num);
        *root = *node;

        DiffNodeDtor(node);
        DiffNodeDtor(root->left);
        DiffNodeDtor(root->right);

        return 1;
    }
    else if (curleft == 1 || curright == 1) {

        if (curleft) {

            if (IsEqDoubleNums(root->left->data.num, 1)) {

                node_t node = *root->right;

                DiffNodeDtor    (root->left);
                FreeOnlyThisNode(root->right);

                *root = node;
            }
            else if (IsEqDoubleNums(root->left->data.num, 0)) {

                node_t *node = NumNodeCtor(0);
                
                DiffNodeDtor(root->left);
                DiffNodeDtor(root->right);

                *root = *node;

                DiffNodeDtor(node);

                return 1;   
            }
        }
        else {

            if (IsEqDoubleNums(root->right->data.num, 1)) {

                node_t node = *root->left;

                FreeOnlyThisNode(root->left);
                DiffNodeDtor    (root->right);

                *root = node;
            }
            else if (IsEqDoubleNums(root->right->data.num, 0)) {

                node_t *node = NumNodeCtor(0);
                
                DiffNodeDtor(root->left);
                DiffNodeDtor(root->right);

                *root = *node;

                DiffNodeDtor(node);

                return 1;
            }
        }
    }

    return 0;
}

int CurtailDiv (node_t *root) {

    int curleft  = CurtailConsts(root->left);
    int curright = CurtailConsts(root->right);

    if (curleft == 1 && curright == 1) {

        node_t *node = NumNodeCtor(root->left->data.num / root->right->data.num);
        *root = *node;

        DiffNodeDtor(node);
        DiffNodeDtor(root->left);
        DiffNodeDtor(root->right);

        return 1;
    }
    else if (curleft == 1 || curright == 1) {

        if (curleft && IsEqDoubleNums(root->left->data.num, 0)) {

            node_t *node = NumNodeCtor(0);
            
            DiffNodeDtor(root->left);
            DiffNodeDtor(root->right);

            *root = *node;

            DiffNodeDtor(node);

            return 1;   
        }
        else if (curright && IsEqDoubleNums(root->right->data.num, 1)) {

            node_t node = *root->left;

            FreeOnlyThisNode(root->left);
            DiffNodeDtor    (root->right);

            *root = node;
        }
    }

    return 0;
}

int CurtailPow (node_t *root) {

    int curleft  = CurtailConsts(root->left);
    int curright = CurtailConsts(root->right);

    if (curleft == 1 && curright == 1) {

        node_t *node = NumNodeCtor(pow(root->left->data.num, root->right->data.num));
        *root = *node;

        DiffNodeDtor(node);
        DiffNodeDtor(root->left);
        DiffNodeDtor(root->right);

        return 1;
    }
    else if (curleft == 1 || curright == 1) {

        if ((curleft && IsEqDoubleNums(root->left->data.num, 1)) || (curright && IsEqDoubleNums(root->right->data.num, 0))) {

            node_t *node = NumNodeCtor(1);
            
            DiffNodeDtor(root->left);
            DiffNodeDtor(root->right);

            *root = *node;

            DiffNodeDtor(node);

            return 1;   
        }
        else if (curright && IsEqDoubleNums(root->right->data.num, 1)) {

            node_t node = *root->left;

            FreeOnlyThisNode(root->left);
            DiffNodeDtor    (root->right);

            *root = node;
        }
    }

    return 0;
} 

