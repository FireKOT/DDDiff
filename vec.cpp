#include <stdio.h>
#include <malloc.h>

#include "vec.h"
#include "config.h"
#include "diff.h"


vec_t VecCtor (int size) {

    vec_t err {
        .data     = nullptr,
        .capacity = -1,
        .size     = -1,
    };

    RET_ON_VAL(size < 0, ERR_INVAL_ARG, err);
    if (size == 0) size = 1;

    vec_t vec {
        .data     = (vecelem_t*) calloc (sizeof(vecelem_t), size),
        .capacity = size,
        .size     = 0,
    };
    RET_ON_VAL(!vec.data, ERR_SEG_FAULT, err);

    return vec;
}

void PushBack (vec_t *vec, vecelem_t num) {

    RET_ON_VAL(!vec, ERR_INVAL_ARG, );

    if (vec->size == vec->capacity) {
        VecResizeUp (vec);
    }

    vec->data[vec->size] = num;
    vec->size++;
}

vecelem_t PopBack (vec_t *vec) {

    lexnode_t err {
        .val = 0,
        .op  = 0,
        .pos = 0,
        .num = 0,
        .var = {},
    };

    RET_ON_VAL(!vec, ERR_INVAL_ARG, err);
    RET_ON_VAL(vec->size <= 0, ERR_SEG_FAULT, err);

    vec->size--;
    if (vec->capacity > 4 && vec->size <= vec->capacity / 4) {
        VecResizeDown(vec);
    }

    return vec->data[vec->size];
}

void VecResizeUp (vec_t *vec) {

    RET_ON_VAL(!vec, ERR_INVAL_ARG, );

    vec->data = (vecelem_t*) realloc(vec->data, sizeof(vecelem_t) * vec->capacity * 2);
    RET_ON_VAL(!vec->data, ERR_SEG_FAULT, );

    vec->capacity *= 2;
}

void VecResizeDown (vec_t *vec) {
    
    RET_ON_VAL(!vec, ERR_INVAL_ARG, );

    vec->data = (vecelem_t*) realloc(vec->data, sizeof(vecelem_t) * vec->capacity / 2);
    RET_ON_VAL(!vec->data, ERR_SEG_FAULT, );

    vec->capacity /= 2;
}

void VecDtor (vec_t *vec) {

    RET_ON_VAL(!vec, ERR_INVAL_ARG, );

    free(vec->data);
    vec->capacity = -1;
    vec->size = -1;
}