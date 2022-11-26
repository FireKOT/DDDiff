#ifndef _SYNTAX_ANAL_H
#define _SYNTAX_ANAL_H

#include "diff.h"


node_t *StartSyntaxAnal (const char * const str);
node_t *GetSumExpr      (const char **str);
node_t *GetMulExpr      (const char **str);
node_t *GetBrackets     (const char **str);
node_t *GetNumber       (const char **str);


#endif