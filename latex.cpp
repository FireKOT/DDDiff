#include <stdio.h>
#include <windows.h>

#include "latex.h"
#include "config.h"
#include "general.h"


static const int PhrasesC = 10;

static const char *Phrases[PhrasesC] = {
    "Очевидно, что",
    "Знающий читатель заметит, что",
    "Понимающий бдитель науки осознает, что",
    "Здравомыслящий человек знает, что",
    "Если задуматься, то станет понятно, что",
    "Оставим читателю в качестве упражения доказать, что это эквивалентно",
    "Я не понимаю что тут написано, но",
    "Где я?",
    "Кто здесь?",
    "Мотьематичненькое преобразование",
};


void InitLaTex (const char *file_name) {

    char name[STRS] = {};
    sprintf(name, "LaTex\\%s", file_name);

    FILE *LaTex = fopen(name, "w");
    RET_ON_VAL(!LaTex, ERR_INCRR_FILE, );

    char *head = GetText("LaTexHead.txt");

    fprintf(LaTex,  "%s\n", head);

    FreeText(head);

    fclose(LaTex);
}

void PrintToLaTex (node_t *root, const char *file_name) {

    char name[STRS] = {};
    sprintf(name, "LaTex\\%s", file_name);

    FILE *LaTex = fopen(name, "a");
    RET_ON_VAL(!LaTex, ERR_INCRR_FILE, );

    fprintf(LaTex, "%s\n", Phrases[rand() % PhrasesC]);

    fprintf(LaTex,  "\\newline\n");
    fprintf(LaTex,  "$ ");
    PrintTree(root, LaTex);
    fprintf(LaTex,  " $\n\n");

    fclose(LaTex);
}

void PrintTree (node_t *node, FILE *output) {

    int op = 0;
    if (node->val == VAL_OP) op = node->data.op;
    else op = OP_NONE;

    switch (op){
        case OP_NONE:
            if (node->val == VAL_NUM) {
                if (node->data.num < 0) fprintf(output, "(");
                fprintf(output, "%lg", node->data.num);
                if (node->data.num < 0) fprintf(output, ")");
            }
            else if (node->val == VAL_VAR) fprintf(output, "%s", node->data.var);

            break;

        case OP_ADD:
            PrintTree(node->left, output);
            fprintf(output, " + ");
            PrintTree(node->right, output);

            break;

        case OP_SUB:
            PrintTree(node->left, output);
            fprintf(output, " - ");
            PrintTree(node->right, output);

            break;

        case OP_MUL:{
            int IsFirstBrackets = false, IsSecondBrackets = false;

            if (node->left->val == VAL_OP && (node->left->data.op == OP_ADD || node->left->data.op == OP_SUB)) IsFirstBrackets = true;
            if (node->right->val == VAL_OP && (node->right->data.op == OP_ADD || node->right->data.op == OP_SUB)) IsSecondBrackets = true;
            
            if (IsFirstBrackets) fprintf(output, "(");
            PrintTree(node->left, output);
            if (IsFirstBrackets) fprintf(output, ")");

            fprintf(output, " \\cdot ");

            if (IsSecondBrackets) fprintf(output, "(");
            PrintTree(node->right, output);
            if (IsSecondBrackets) fprintf(output, ")");

            break;
        }

        case OP_DIV:
            fprintf(output, " \\frac{");
            PrintTree(node->left, output);
            fprintf(output, "}{");
            PrintTree(node->right, output);
            fprintf(output, "} ");

            break;

        case OP_LN:
            fprintf(output, " \\ln{");
            PrintTree(node->right, output);
            fprintf(output, "} ");

            break;
        
        case OP_POW:
            fprintf(output, " {");
            if (node->left->val != VAL_NUM && node->left->val != VAL_VAR) fprintf(output, "(");
            PrintTree(node->left, output);
            if (node->left->val != VAL_NUM && node->left->val != VAL_VAR) fprintf(output, ")");
            fprintf(output, "}^{");
            PrintTree(node->right, output);
            fprintf(output, "} ");

            break;

        case OP_SIN:
            fprintf(output, " \\sin{");
            if (node->right->val != VAL_NUM && node->right->val != VAL_VAR) fprintf(output, "(");
            PrintTree(node->right, output);
            if (node->right->val != VAL_NUM && node->right->val != VAL_VAR) fprintf(output, ")");
            fprintf(output, "} ");

            break;
        
        case OP_COS:
            fprintf(output, " \\cos{");
            if (node->right->val != VAL_NUM && node->right->val != VAL_VAR) fprintf(output, "(");
            PrintTree(node->right, output);
            if (node->right->val != VAL_NUM && node->right->val != VAL_VAR) fprintf(output, ")");
            fprintf(output, "} ");

            break;

        case OP_TG:
            fprintf(output, " \\tg{");
            if (node->right->val != VAL_NUM && node->right->val != VAL_VAR) fprintf(output, "(");
            PrintTree(node->right, output);
            if (node->right->val != VAL_NUM && node->right->val != VAL_VAR) fprintf(output, ")");
            fprintf(output, "} ");

            break;

        case OP_CTG:
            fprintf(output, " \\ctg{");
            if (node->right->val != VAL_NUM && node->right->val != VAL_VAR) fprintf(output, "(");
            PrintTree(node->right, output);
            if (node->right->val != VAL_NUM && node->right->val != VAL_VAR) fprintf(output, ")");
            fprintf(output, "} ");

            break;

        case OP_ARCSIN:
            fprintf(output, " \\arcsin{");
            if (node->right->val != VAL_NUM && node->right->val != VAL_VAR) fprintf(output, "(");
            PrintTree(node->right, output);
            if (node->right->val != VAL_NUM && node->right->val != VAL_VAR) fprintf(output, ")");
            fprintf(output, "} ");

            break;
        
        case OP_ARCCOS:
            fprintf(output, " \\arccos{");
            if (node->right->val != VAL_NUM && node->right->val != VAL_VAR) fprintf(output, "(");
            PrintTree(node->right, output);
            if (node->right->val != VAL_NUM && node->right->val != VAL_VAR) fprintf(output, ")");
            fprintf(output, "} ");

            break;

        case OP_ARCTG:
            fprintf(output, " \\arctg{");
            if (node->right->val != VAL_NUM && node->right->val != VAL_VAR) fprintf(output, "(");
            PrintTree(node->right, output);
            if (node->right->val != VAL_NUM && node->right->val != VAL_VAR) fprintf(output, ")");
            fprintf(output, "} ");

            break;

        case OP_ARCCTG:
            fprintf(output, " \\arcctg{");
            if (node->right->val != VAL_NUM && node->right->val != VAL_VAR) fprintf(output, "(");
            PrintTree(node->right, output);
            if (node->right->val != VAL_NUM && node->right->val != VAL_VAR) fprintf(output, ")");
            fprintf(output, "} ");

            break;
        
        default:
            break;
    }
}

void MakeMeth (const char *file_name) {

    char name[STRS] = {};
    sprintf(name, "LaTex\\%s", file_name);

    FILE *LaTex = fopen(name, "a");
    RET_ON_VAL(!LaTex, ERR_INCRR_FILE, );

    fprintf(LaTex,  "\\end{document}\n");

    fclose(LaTex);

    char cmd[STRS] = {};
    sprintf(cmd, "cd LaTex && pdflatex %s -quiet", file_name);

    system(cmd);
}