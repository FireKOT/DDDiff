DEF_OP (NONE, 0, "", 0)

DEF_OP (ADD, 1, "+", 1, {
    root = GetUnoExpr(lexnodes);
})

DEF_OP (SUB, 2, "-", 1, {
    root = OpNodeCtor(OP_MUL);
    root->left  = NumNodeCtor(-1);
    root->right = GetUnoExpr(lexnodes);

    if (!root->right) return nullptr;
})

DEF_OP (MUL, 3, "*", 0)

DEF_OP (DIV, 4, "/", 0)

DEF_OP (POW, 5, "^", 0)

DEF_OP (OBRACKET, 6, "(", 0)

DEF_OP (CBRACKET, 7, ")", 0)

DEF_OP (SIN, 8, "sin", 1, {
    root = OpNodeCtor(OP_SIN);
    root->right = GetUnoExpr(lexnodes);

    if (!root->right) return nullptr;
})

DEF_OP (COS, 9, "cos", 1, {
    root = OpNodeCtor(OP_COS);
    root->right = GetUnoExpr(lexnodes);

    if (!root->right) return nullptr;
})

DEF_OP (TG, 10, "tg", 1, {
    root = OpNodeCtor(OP_TG);
    root->right = GetUnoExpr(lexnodes);

    if (!root->right) return nullptr;
})

DEF_OP (CTG, 11, "ctg", 1, {
    root = OpNodeCtor(OP_CTG);
    root->right = GetUnoExpr(lexnodes);

    if (!root->right) return nullptr;
})

DEF_OP (ARCSIN, 12, "arcsin", 1, {
    root = OpNodeCtor(OP_ARCSIN);
    root->right = GetUnoExpr(lexnodes);

    if (!root->right) return nullptr;
})

DEF_OP (ARCCOS, 13, "arccos", 1, {
    root = OpNodeCtor(OP_ARCCOS);
    root->right = GetUnoExpr(lexnodes);

    if (!root->right) return nullptr;
})

DEF_OP (ARCTG, 14, "arctg", 1, {
    root = OpNodeCtor(OP_ARCTG);
    root->right = GetUnoExpr(lexnodes);

    if (!root->right) return nullptr;
})

DEF_OP (ARCCTG, 15, "arcctg", 1, {
    root = OpNodeCtor(OP_ARCCTG);
    root->right = GetUnoExpr(lexnodes);

    if (!root->right) return nullptr;
})

DEF_OP (LN, 16, "ln", 1, {
    root = OpNodeCtor(OP_LN);
    root->right = GetUnoExpr(lexnodes);

    if (!root->right) return nullptr;
})