#include "sphere.h"
// #ifdef __cmakelist
// else
    // #include "kernel_gpu/sphere.h"
// #endif

#ifndef STRUCT_H_
    #define STRUCT_H_

enum primitive_e {
    SHPERE = 1,
};

struct primitive {
    enum primitive_e type;
    union {
        struct sphere sphere;
    } data;
};

#endif /* !STRUCT_H_ */
