#ifndef _SYNTAX_ANAL_H
#define _SYNTAX_ANAL_H


#include "vec.h"


node_t *StartSyntaxAnal (const char *expr);
node_t *GetSumExpr      (lexnode_t **lexnodes);
node_t *GetMulExpr      (lexnode_t **lexnodes);
node_t *GetUnoExpr      (lexnode_t **lexnodes);
node_t *GetPowExpr      (lexnode_t **lexnodes);
node_t *GetBrackets     (lexnode_t **lexnodes);

vec_t LexicalAnalysis (const char * const str);


#endif