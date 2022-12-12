#ifndef _VEC_H
#define _VEC_H


#include "diff.h"


typedef lexnode_t vecelem_t;


struct vec_t {

    vecelem_t *data;
    int capacity, size;
};


vec_t VecCtor      (int size = 1024);
void PushBack      (vec_t *vec, vecelem_t num);
vecelem_t PopBack  (vec_t *vec);
void VecResizeUp   (vec_t *vec);
void VecResizeDown (vec_t *vec);
void VecDtor       (vec_t *vec);


#endif