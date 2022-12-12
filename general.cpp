#include <stdio.h>
#include <tgmath.h>
#include <sys/stat.h>
#include <malloc.h>

#include "general.h"
#include "config.h"

static double EPS = 1e-6;

int IsEqDoubleNums  (double n, double m) {

    return fabs(n - m) < EPS;
}

int ClearBuf () {

    int trash = false;
    int c = 0;
    while((c = getchar()) != '\n'){
        if (c != ' ' && c != '\t'){
            trash = true;
        }
    }

    return trash;
}

int WantContinue () {

    printf("Do you want to continue [Y/N]?\n");

    int c = getchar();
    int isClear = ClearBuf();
    while((c != 'Y' && c != 'y' && c != 'N' && c != 'n') || isClear != 0){
        printf("Incorrect value\n");
        printf("Do you want to continue [Y/N]?\n");

        c = getchar();
        isClear = ClearBuf();
    }

    if(c == 'Y' || c =='y'){
        return 1;
    }

    return 0;
}

int GetFileSize (const char *file_name) {

    struct stat file_stats;
    int status = stat(file_name, &file_stats);
    RET_ON_VAL(status == -1, ERR_ACC_DENi, -1);

    return file_stats.st_size;
}

char *GetText (const char *file_name) {

    FILE *input = fopen(file_name, "r");
    RET_ON_VAL(!input, ERR_INCRR_FILE, nullptr);

    int file_size = GetFileSize(file_name);
    char *buf = (char*) calloc(file_size, sizeof(char));

    fread(buf, sizeof(char), file_size, input);

    return buf;
}

void FreeText (char *text) {

    free(text);
}
