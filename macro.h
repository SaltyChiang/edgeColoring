#ifndef MACRO_H
#define MACRO_H

#include <cstdlib>
#include <cstdio>

#define EPS 1e-14

typedef int label;
typedef double scalar;

#define static_malloc(_typename_, _count_)                                                         \
    static_cast<_typename_ *>(malloc((_count_) * sizeof(_typename_)))

#define static_calloc(_typename_, _count_)                                                         \
    static_cast<_typename_ *>(calloc(_count_, sizeof(_typename_)))

typedef struct Forest_s {
    label *row;
    label *col;
    label *forestIdx;
    label Delta;
} Forest;

typedef struct Root_s {
    label *root;
    label *rootIdx;
    label Delta;
} Root;

#endif